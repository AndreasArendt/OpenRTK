LoadData;

plotCov = false;

usePreciseEph = false;

refpos__m = [NaN, NaN, NaN];

pos_E__m           = zeros(numel(SatelliteData), 3);
rx_clock_offset__m = zeros(numel(SatelliteData), 1);
zwd__m             = zeros(numel(SatelliteData), 1); %zenith wet delay

P_pos = NaN(numel(SatelliteData), 3); % covariance for position

for ii = 1:numel(SatelliteData)
    %% Pre-Processing
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);
    
    if usePreciseEph == true
        % TODO: SV order is implicitly used here
        d = Generic.GetPreciseEphemeris(PreciseEphemerisData, {Observations.SatelliteSystem}, SatelliteData(ii).PosixEpochTime__s);
        v = [d.entries('Struct').Value];
        ECEF_x = [v.x].';
        ECEF_y = [v.y].';
        ECEF_z = [v.z].';
    else
        ECEF_Position = [Observations.ECEF_Position];
        ECEF_x = [ECEF_Position.x]';
        ECEF_y = [ECEF_Position.y]';
        ECEF_z = [ECEF_Position.z]';
    end

    %% Algorithm    
    POS_E__M           = pos_E__m(ii,:);
    RX_CLOCK_OFFSET__M = rx_clock_offset__m(ii);
    ZWD__m             = zwd__m(ii);
    for jj = 1:10           
        % Calc Distance 
        dist = Vector.EuclidianDistance_3D(ECEF_x, ECEF_y, ECEF_z, POS_E__M(1), POS_E__M(2), POS_E__M(3));

        % Sagnac Correction
        sag = Transformation.CalcSagnac( [ECEF_x, ECEF_y, ECEF_z], [POS_E__M(1), POS_E__M(2), POS_E__M(3)]);

        % correct geographic distance of earth rotation
        geo_dist = dist + sag;
        
        sv_clock_offset__m = Transformation.SpeedOfLight__mDs * [Observations.ClockOffset]';    
        sv_relativistic__m = Transformation.SpeedOfLight__mDs * [Observations.RelativisticError]';
    
        % elevation mask
        elevation = Generic.CalcElevation(POS_E__M(1), POS_E__M(2), POS_E__M(3), ECEF_x, ECEF_y, ECEF_z);
        
        % Troposphere Model
        [lat__rad, ~, alt__m] = Transformation.ecef2wgs84(POS_E__M(1), POS_E__M(2), POS_E__M(3));    
        tropo_offset = Troposphere.Saastamoinen_TropoModel(lat__rad, alt__m, elevation, ZWD__m);     

        % Iono-Free LC
        Code = [Observations.Code];
        rho_iono_free = Generic.CalcIonoFreeLinearCombination([Code.Band_1]', [Code.Band_2]', F_E1_Galileo__Hz, F_L2_GPS__Hz);
               
        % Design Matrix
        e = Vector.NormalizedDistanceVector(ECEF_x, ECEF_y, ECEF_z, POS_E__M(1), POS_E__M(2), POS_E__M(3));    
        Mw = Troposphere.MappingFunction.Chao_MW(elevation);
        A = [-e, ones(numel(e(:,1)),1), Mw];                
        
        dist_est = rho_iono_free + sv_clock_offset__m + sv_relativistic__m - RX_CLOCK_OFFSET__M - tropo_offset;        
        y = dist_est - geo_dist;

        if (SatelliteData(ii).PosixEpochTime__s - SatelliteData(1).PosixEpochTime__s) >= 191.4            
            a = 1;
        end

        % Apply elevation filter only if more than 4SVs available
        idx_el = elevation > deg2rad(15) & elevation < deg2rad(165);
        if nnz(idx_el) > 4
            A = A(idx_el,:);
            y = y(idx_el);
        end 
        
        [x_hat, P] = estimation.lls(A, y);

        % State update
        POS_E__M           = POS_E__M + x_hat(1:3)';        
        RX_CLOCK_OFFSET__M = RX_CLOCK_OFFSET__M + x_hat(4);
        ZWD__m             = ZWD__m + x_hat(5);
    
        if all(abs(x_hat(1:3)) < 1e-3)
            break;
        end
    end
        
    pos_E__m(ii+1,:)          = POS_E__M;
    rx_clock_offset__m(ii+1)  = RX_CLOCK_OFFSET__M;
    zwd__m(ii+1)              = ZWD__m;

    P_pos(ii+1,:) = diag(P(1:3, 1:3));
end

% adjust off-by-one index
pos_E__m           = pos_E__m(2:end,:);
rx_clock_offset__m = rx_clock_offset__m(2:end);
zwd__m             = zwd__m(2:end);

P_pos = P_pos(2:end,:);

%%
af = afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(refpos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,1), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,1) + sqrt(P_pos(:,1)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,1) - sqrt(P_pos(:,1)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(4,2,2);
hold on; grid on;
plot(refpos__m(1) - pos_E__m(:,1), 'DisplayName', 'LSQ (5 States)')

subplot(4,2,3);
hold on; grid on;
yline(refpos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,2), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,2) + sqrt(P_pos(:,2)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,2) - sqrt(P_pos(:,2)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(4,2,4);
hold on; grid on;
plot(refpos__m(2) - pos_E__m(:,2), 'DisplayName', 'LSQ (5 States)')

subplot(4,2,5);
hold on; grid on;
yline(refpos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,3), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,3) + sqrt(P_pos(:,3)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,3) - sqrt(P_pos(:,3)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(4,2,6);
hold on; grid on;
plot(refpos__m(3) - pos_E__m(:,3), 'DisplayName', 'LSQ (5 States)')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(rx_clock_offset__m ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'LSQ (5 States)')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(zwd__m, 'DisplayName','zwd__m')

af.linkaxes('x');
af.showlegend();
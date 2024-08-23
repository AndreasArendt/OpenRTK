LoadData;

pos_E__m           = zeros(numel(SatelliteData), 3);
rx_clock_offset__m = zeros(numel(SatelliteData), 1);

posix = NaN(size(SatelliteData));

for ii = 1:numel(SatelliteData)
    %% Pre-Processing
    Epoch        = SatelliteData(ii).Epoch;
    Observations = SatelliteData(ii).Observations;

    posix(ii) = epoch2posix( Epoch.day, Epoch.month, Epoch.year, Epoch.hour, Epoch.minute, Epoch.second);

    idx_gps = cellfun(@(x)startsWith(x, 'G'), {Observations.SatelliteSystem});
    idx_gal = cellfun(@(x)startsWith(x, 'E'), {Observations.SatelliteSystem});
    
    Code    = [Observations.Code];

    idx_code_1 = [Code.Band_1] > 0;
    idx_code_2 = [Code.Band_2] > 0;
    idx_code_5 = [Code.Band_5] > 0;

    idx_healthy = [Observations.IsHealthy];

    idx = idx_gps & idx_code_1 & idx_code_2 & idx_healthy;

    %% Algorithm    
    POS_E__M           = pos_E__m(ii,:);
    RX_CLOCK_OFFSET__M = rx_clock_offset__m(ii);
    
    ECEF_Position = [Observations.ECEF_Position];
    ECEF_x = [ECEF_Position(idx).x]';
    ECEF_y = [ECEF_Position(idx).y]';
    ECEF_z = [ECEF_Position(idx).z]';

    for jj = 1:10           
        % Calc Distance 
        dist = Vector.EuclidianDistance_3D(ECEF_x, ECEF_y, ECEF_z, POS_E__M(1), POS_E__M(2), POS_E__M(3));

        % Sagnac Correction
        sag = Transformation.CalcSagnac( [ECEF_x, ECEF_y, ECEF_z], [POS_E__M(1), POS_E__M(2), POS_E__M(3)]);

        % correct geographic distance of earth rotation
        geo_dist = dist + sag;
        
        sv_clock_offset__m = Transformation.SpeedOfLight__mDs * [Observations(idx).ClockOffset]';    
        sv_relativistic__m = Transformation.SpeedOfLight__mDs * [Observations(idx).RelativisticError]';
    
        % elevation mask
        elevation = Generic.CalcElevation(POS_E__M(1), POS_E__M(2), POS_E__M(3), ECEF_x, ECEF_y, ECEF_z);
        
        % Troposphere Model
        tropo_offset = Troposphere.CalcTropoOffset(POS_E__M(1), POS_E__M(2), POS_E__M(3), elevation);
            
        % Iono-Free LC
        rho_iono_free = Generic.CalcIonoFreeLinearCombination([Code(idx).Band_1]', [Code(idx).Band_2]', F_E1_Galileo__Hz, F_L2_GPS__Hz);
                
        e = Vector.NormalizedDistanceVector(ECEF_x, ECEF_y, ECEF_z, POS_E__M(1), POS_E__M(2), POS_E__M(3));    
        A = [-e, ones(numel(e(:,1)),1)];                
        
        dist_est = rho_iono_free + sv_clock_offset__m + sv_relativistic__m - RX_CLOCK_OFFSET__M - tropo_offset;        
        y = dist_est - geo_dist;

        % Apply elevation filter only if more than 4SVs available
        idx_el = elevation > (5/180*pi) & elevation < (175/180*pi);
        if nnz(idx_el) > 4
            A = A(idx_el,:);
            y = y(idx_el);
        end 

        x_hat = (A.'*A)\A.'*y;
        
        % State update
        POS_E__M           = POS_E__M + x_hat(1:3)';        
        RX_CLOCK_OFFSET__M = RX_CLOCK_OFFSET__M + x_hat(4);

        if all(abs(x_hat(1:3)) < 1e-3)
            break;
        end
    end
        
    pos_E__m(ii+1,:)          = POS_E__M;
    rx_clock_offset__m(ii+1)  = RX_CLOCK_OFFSET__M;
end

% adjust off-by-one index
pos_E__m           = pos_E__m(2:end,:);
rx_clock_offset__m = rx_clock_offset__m(2:end);

%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(station_pos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,1), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,2);
hold on; grid on;
plot(station_pos__m(1) - pos_E__m(:,1), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,3);
hold on; grid on;
yline(station_pos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,2), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,4);
hold on; grid on;
plot(station_pos__m(2) - pos_E__m(:,2), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,5);
hold on; grid on;
yline(station_pos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,3), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,6);
hold on; grid on;
plot(station_pos__m(3) - pos_E__m(:,3), 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(rx_clock_offset__m ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(NaN, 'HandleVisibility','off')

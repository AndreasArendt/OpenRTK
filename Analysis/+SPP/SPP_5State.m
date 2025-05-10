% LoadData;

pos_E__m              = zeros(numel(SatelliteData), 3);
rx_clock_offset__m    = zeros(numel(SatelliteData), 1);
zenith_tropo_delay__m = zeros(numel(SatelliteData), 1);

n_sat         = NaN(numel(SatelliteData), 1); % number of satellites used
elevation_out = NaN(numel(SatelliteData), meta.NumberSatellites);

for ii = 1:numel(SatelliteData)
    %% Pre-Processing       
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);

    if numel(Observations) < 4
        pos_E__m(ii+1,:)         = POS_E__M;
        rx_clock_offset__m(ii+1) = RX_CLOCK_OFFSET__M;
        n_sat(ii+1)              = numel(Observations);
        continue;
    end

    if ii >= 119
        a = 1;
    end

    %% Algorithm    
    POS_E__M              = pos_E__m(ii,:);
    RX_CLOCK_OFFSET__M    = rx_clock_offset__m(ii);
    ZENITH_TROPO_DELAY__M = zenith_tropo_delay__m(ii);

    ECEF_Position = [Observations.ECEF_Position];
    ECEF_x = [ECEF_Position.x]';
    ECEF_y = [ECEF_Position.y]';
    ECEF_z = [ECEF_Position.z]';
    
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
        elevation = generic.calcElevation(POS_E__M(1), POS_E__M(2), POS_E__M(3), ECEF_x, ECEF_y, ECEF_z);
        
        % Troposphere model - wet component estimated
        [~, ~, alt__m] = Transformation.ecef2wgs84(POS_E__M(1), POS_E__M(2), POS_E__M(3));

        d_dry = 2.3 .* exp(-0.116e-3 .* alt__m);
    
        if isinf(d_dry)
            d_dry = 1e9;
        end
          
        d_wet = 0.1 + ZENITH_TROPO_DELAY__M;        
        M_E = 1.001./sqrt(0.002001 + sin(elevation).^2);                        
        tropo_offset = (d_dry + d_wet) .* M_E;

        % Iono-Free LC
        Code = [Observations.Code];
        rho_iono_free = generic.calcIonoFreeLinearCombination([Code.Band_1]', [Code.Band_2]', gnss.F_L1_GPS__Hz, gnss.F_L2_GPS__Hz);
                
        e = Vector.NormalizedDistanceVector(ECEF_x, ECEF_y, ECEF_z, POS_E__M(1), POS_E__M(2), POS_E__M(3));                       
        A = [-e, ones(numel(e(:,1)),1), M_E];
       
        dist_est = rho_iono_free + sv_clock_offset__m + sv_relativistic__m - RX_CLOCK_OFFSET__M - tropo_offset;        
        y = dist_est - geo_dist;
       
        % Apply elevation filter only if more than 4SVs available
        idx_el = elevation > (5/180*pi) & elevation < (175/180*pi);
        if nnz(idx_el) > 4
            A = A(idx_el,:);
            y = y(idx_el);      
        end 
        
        [x_hat, P, res] = estimation.lls(A, y);        

        % State update
        POS_E__M              = POS_E__M + x_hat(1:3)';        
        RX_CLOCK_OFFSET__M    = RX_CLOCK_OFFSET__M + x_hat(4);
        ZENITH_TROPO_DELAY__M = ZENITH_TROPO_DELAY__M + x_hat(5);

        if all(abs(x_hat(1:3)) < 1e-5)
            break;
        end
    end
        
    pos_E__m(ii+1,:)            = POS_E__M;
    rx_clock_offset__m(ii+1)    = RX_CLOCK_OFFSET__M;
    zenith_tropo_delay__m(ii+1) = ZENITH_TROPO_DELAY__M;

    n_sat(ii+1)             = nnz(idx_el);
end

% adjust off-by-one index
pos_E__m              = pos_E__m(2:end,:);
rx_clock_offset__m    = rx_clock_offset__m(2:end);
zenith_tropo_delay__m = zenith_tropo_delay__m(2:end);

%%
af = afigure(42);
subplot(5,2,1);
hold on; grid on;
yline(refpos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,1), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,2);
hold on; grid on;
plot(refpos__m(1) - pos_E__m(:,1), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,3);
hold on; grid on;
yline(refpos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,2), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,4);
hold on; grid on;
plot(refpos__m(2) - pos_E__m(:,2), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,5);
hold on; grid on;
yline(refpos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,3), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,6);
hold on; grid on;
plot(refpos__m(3) - pos_E__m(:,3), 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,7);
hold on; grid on;
title('dt_r')
plot(rx_clock_offset__m ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,8);
hold on; grid on;
title('ZTD_w', 'DisplayName', 'LSQ (5 States, SPP)')
plot(zenith_tropo_delay__m, 'DisplayName', 'LSQ (5 States, SPP)')
legend('show')

subplot(5,2,9);
hold on; grid on;
title('nSat')
plot(n_sat, 'DisplayName', 'nsat (5 States, SPP)');

subplot(5,2,10);
hold on; grid on;
title('elevations')

af.linkaxes('x');
af.showlegend();
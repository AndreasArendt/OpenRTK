LoadData;

pos = [0,0,0];
cdtr = 0;

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
    POS  = pos(ii,:);
    CDTR = cdtr(ii);
    
    ECEF_Position = [Observations.ECEF_Position];
    ECEF_x = [ECEF_Position(idx).x]';
    ECEF_y = [ECEF_Position(idx).y]';
    ECEF_z = [ECEF_Position(idx).z]';

    for jj = 1:10           
        % Calc Distance 
        dist = Vector.EuclidianDistance_3D(ECEF_x, ECEF_y, ECEF_z, POS(1), POS(2), POS(3));

        % Sagnac Correction
        sag = Transformation.CalcSagnac( [ECEF_x, ECEF_y, ECEF_z], [POS(1), POS(2), POS(3)]);

        % correct geographic distance of earth rotation
        geo_dist = dist + sag;
        
        cdt_sv  = Transformation.SpeedOfLight__mDs * [Observations(idx).ClockOffset]';    
        cdt_rel = Transformation.SpeedOfLight__mDs * [Observations(idx).RelativisticError]';
    
        % elevation mask
        elevation = Generic.CalcElevation(POS(1), POS(2), POS(3), ECEF_x, ECEF_y, ECEF_z);

        idx_el = elevation > (5/180*pi) & elevation < (175/180*pi);

        % Troposphere Model
        tropo_offset = Troposphere.CalcTropoOffset(POS(1), POS(2), POS(3), elevation);
            
        % Iono-Free LC
        rho_iono_free = Generic.CalcIonoFreeLinearCombination([Code(idx).Band_1]', [Code(idx).Band_2]', F_E1_Galileo__Hz, F_L2_GPS__Hz);
                
        dist_est = rho_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset;        
        
        e = Vector.NormalizedDistanceVector(ECEF_x, ECEF_y, ECEF_z, POS(1), POS(2), POS(3));
    
        A = [-e, ones(numel(e(:,1)),1)];
                
        y = dist_est - geo_dist;

        % Apply elevation filter only if more than 4SVs available
        if nnz(idx_el) > 4
            A = A(idx_el,:);
            y = y(idx_el);
        end 

        x_hat = (A.'*A)\A.'*y;
        
        POS = POS + x_hat(1:3)';        
        CDTR = CDTR + x_hat(4);

        if all(abs(x_hat(1:3)) < 1e-3)
            break;
        end
    end
        
    pos(ii+1,:) = POS;
    cdtr(ii+1)  = CDTR;
end

%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(station_pos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,1), 'DisplayName', 'LSQ (4 States)')
ylim([station_pos__m(1)-500,  station_pos__m(1)+500])
legend('show')

subplot(4,2,2);
hold on; grid on;
plot(station_pos__m(1) - pos(:,1), 'DisplayName', 'LSQ (4 States)')
ylim([-20 20])
legend('show')

subplot(4,2,3);
hold on; grid on;
yline(station_pos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,2), 'DisplayName', 'LSQ (4 States)')
ylim([station_pos__m(2)-500, station_pos__m(2)+500])
legend('show')

subplot(4,2,4);
hold on; grid on;
plot(station_pos__m(2) - pos(:,2), 'DisplayName', 'LSQ (4 States)')
ylim([-20 20])
legend('show')

subplot(4,2,5);
hold on; grid on;
yline(station_pos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,3), 'DisplayName', 'LSQ (4 States)')
ylim([station_pos__m(3)-500, station_pos__m(3)+500])
legend('show')

subplot(4,2,6);
hold on; grid on;
plot(station_pos__m(3) - pos(:,3), 'DisplayName', 'LSQ (4 States)')
ylim([-20 20])
legend('show')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(cdtr ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'LSQ (4 States)')
legend('show')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(NaN, 'HandleVisibility','off')

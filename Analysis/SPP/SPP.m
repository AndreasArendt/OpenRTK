LoadData;

idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

pos = [0,0,0];
cdtr = 0;

ctr = 1;

idx_Galileo = cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph)                          .* false;
idx_GPS     = cellfun(@(x)strcmp(x(1), 'G'), S.SvSystem_t_eph)                          .* true;

% take care, this has been reworked to use only GPS L1/L2!!!

timestamps = unique(S.ObsToc);
for tt = timestamps.'   
    idx = S.ObsToc == tt;

    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & (idx_Galileo | idx_GPS) & ...   % Galileo only!
                       (S.Code_1 > 1e3) & (S.Code_2 > 1e3) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 2));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);
    code_5_idx = code_idx & (S.Band == 2);
    
    [ia,ib] = ismember(S.SvSystemID_t_eph(code_1_idx), S.SvSystemID_t_eph(code_5_idx));
    tmp = find(code_1_idx);
    code_1_idx(tmp(~ia)) = 0;
    
    POS = pos(ctr,:);
    CDTR = cdtr(ctr);
    
    for ii = 1:10           
        % Calc Distance 
        dist = Vector.EuclidianDistance_3D(S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                  POS(1), POS(2), POS(3));

        % Sagnac Correction
        sag = Transformation.CalcSagnac( [S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx)], ...
                                         [POS(1), POS(2), POS(3)]);

        % correct geographic distance of earth rotation
        geo_dist = dist + sag;
        
        cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
        cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);
    
        % elevation mask
        elevation = Generic.CalcElevation(POS(1), POS(2), POS(3), S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx));

        idx_el = elevation > (5/180*pi) & elevation < (175/180*pi);

        % Troposphere Model
        tropo_offset = CalcTropoOffset(POS(1), POS(2), POS(3), elevation);
            
        % Iono-Free LC
        rho_iono_free = Generic.CalcIonoFreeLinearCombination(S.Code_1(code_1_idx), S.Code_2(code_5_idx), F_E1_Galileo__Hz, F_L2_GPS__Hz);
                
        dist_est = rho_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset;        
        
        e = Vector.NormalizedDistanceVector(S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                            POS(1), POS(2), POS(3));
    
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
        
    ctr = ctr+1;
    pos(ctr,:) = POS;
    cdtr(ctr) = CDTR;
end

%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(station_pos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,1), 'DisplayName', 'LSQ (4 States)')
ylim([station_pos__m(1)-50,  station_pos__m(1)+50])
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
ylim([station_pos__m(2)-50, station_pos__m(2)+50])
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
ylim([station_pos__m(3)-50, station_pos__m(3)+50])
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

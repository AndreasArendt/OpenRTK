clear; 
init;

idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

ctr = 1;

idx_Galileo = cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph);
idx_GPS     = cellfun(@(x)strcmp(x(1), 'G'), S.SvSystem_t_eph) .* 0;

idx_SV = idx_Galileo | idx_GPS;

svID = unique(S.SvSystemID_t_eph(idx_SV)); 
nAmbiguity = numel(svID);
nStates = 5 + nAmbiguity;

POS    = station_pos__m;
CDTR   = 0;
ZTD    = 0;
AMBIG  = zeros(nAmbiguity,1);

x_hat = [POS';CDTR;ZTD;AMBIG];

pos(1,:)   = POS;
cdtr(1)    = CDTR;
ztd(1)     = ZTD;
ambig(1,:) = NaN(1,nAmbiguity);

timestamps = unique(S.ObsToc);
for tt = timestamps.'   
    idx = S.ObsToc == tt;

    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & (idx_Galileo | idx_GPS) & ...
                       (S.Code_1 > 1e3) & (S.Code_1 > 1e3) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);
    code_5_idx = code_idx & (S.Band == 5);
    
    phase_idx   = idx & (idx_Galileo | idx_GPS) & ...
                       (S.Phase_1 > 0) & (S.Code_5 > 0) & ...                   % valid Carrierphases
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    phase_1_idx = phase_idx & (S.Band == 1);
    phase_5_idx = phase_idx & (S.Band == 5);

    [ia,ib] = ismember(S.SvSystemID_t_eph(code_1_idx), S.SvSystemID_t_eph(code_5_idx));
    tmp = find(code_1_idx);
    code_1_idx(tmp(~ia)) = 0;
    
    [ia,ib] = ismember(S.SvSystemID_t_eph(phase_1_idx), S.SvSystemID_t_eph(phase_5_idx));
    tmp = find(phase_1_idx);
    phase_1_idx(tmp(~ia)) = 0;

    idx_Amb = ismember(svID, unique(S.SvSystemID_t_eph(phase_idx))); %index of Ambiguity data in State vector

    for ii = 1:5
        % Calc Distance 
        dist = Vector.EuclidianDistance_3D(S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                           POS(1), POS(2), POS(3));
    
        % Sagnac Correction
        sag = Transformation.CalcSagnac( [S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx)], ...
                                         [POS(1), POS(2), POS(3)]);
    
        geo_dist = dist + sag;
    
        % distance Vector & normalized distance Vector
        d = Vector.DistanceVector_3D(S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                     POS(1), POS(2), POS(3));
    
        e = Vector.NormalizedDistanceVector(S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                            POS(1), POS(2), POS(3));
    
        % SV Clock Error & Relativistic effects
        cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
        cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);
    
        % elevation mask
        elevation = Generic.CalcElevation(POS(1), POS(2), POS(3), d(:,1), d(:,2), d(:,3));
            
        idx_el = elevation > (15/180*pi) & elevation < (165/180*pi);    
        
        % Troposphere
        [lat__rad, lon__rad, alt__m] = Transformation.ecef2wgs84(POS(1), POS(2), POS(3));
    
        d_dry = 2.3 .* exp(-0.116e-3 .* alt__m);
    
        if isinf(d_dry)
            d_dry = 1e9;
        end
        
        d_wet = 0.1 + ZTD;        
        M_E = 1.001./sqrt(0.002001 + sin(elevation).^2);                        
        tropo_offset = (d_dry + d_wet) .* M_E;
               
        % Iono-Free LC
        rho_iono_free = Generic.CalcIonoFreeLinearCombination(S.Code_1(code_1_idx), S.Code_5(code_5_idx), F_E1_Galileo__Hz, F_E5a_Galileo__Hz);
        phi_iono_free = Generic.CalcIonoFreeLinearCombination(S.Phase_1(phase_1_idx), S.Phase_5(phase_5_idx), F_E1_Galileo__Hz, F_E5a_Galileo__Hz);
    
        % get Carrierphase related data
        ia = 154; % see: TheImplementationOfPPP_AComprehensiveReview (34)
        ib = -115;
        
        lambda_E1  = Transformation.SpeedOfLight__mDs / F_E1_Galileo__Hz;
        lambda_E5a = Transformation.SpeedOfLight__mDs / F_E5a_Galileo__Hz;
        lambda_IF = (lambda_E1*lambda_E5a) / ((ia*lambda_E5a) + (ib*lambda_E1));
                
        pseudorange_est  = rho_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset;            
        carrierphase_est = phi_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset - AMBIG(idx_Amb);
    
        nObs = numel(e(:,1));

        % get number of active states (n + active Ambiguity)
        nActiveStates = nStates - nAmbiguity + nnz(idx_Amb);

        H_rho = [-e, ones(nObs,1), M_E, zeros(nObs, nnz(idx_Amb))];
        H_phi = zeros(nnz(idx_Amb), nActiveStates);
        H_phi(:,1:3) = -e;
        H_phi(:,4) = 1;
        H_phi(:,5) = M_E;       
        H_phi(:, 6:end) = eye(nnz(idx_Amb)); 
    
        H = [H_rho; H_phi];
    
        idx_zero = all(H == 0);
        H(:,idx_zero)=[];
    
        y = [ pseudorange_est  - geo_dist; ...
              carrierphase_est - geo_dist];
    
        A = H.' * H;
        b = -H.' * y;
        x = -inv(H.'*H) * H.' * y;
    
        % x_hat = (A.'*A)\A.'*b;
        x_hat = (H.'*H)\H.'*y;

        POS            = POS + x_hat(1:3)';                
        CDTR           = CDTR + x_hat(4);
        ZTD            = ZTD + x_hat(5);
        AMBIG(idx_Amb) = AMBIG(idx_Amb) + x_hat(6:end); 
    end

    ctr = ctr+1;
    pos(ctr,:) = POS;
    cdtr(ctr) = CDTR;
    ztd(ctr) = ZTD;
    ambig(ctr,:) = NaN(1,nAmbiguity);
    ambig(ctr,idx_Amb) = AMBIG(idx_Amb);
end
         
% ReadRtkPos;
% return;
%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(station_pos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,1), 'DisplayName', 'EKF (5 States)')
ylim([station_pos__m(1)-50,  station_pos__m(1)+50])
legend('show')

subplot(4,2,2);
hold on; grid on;
plot(station_pos__m(1) - pos(:,1), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,3);
hold on; grid on;
yline(station_pos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,2), 'DisplayName', 'EKF (5 States)')
ylim([station_pos__m(2)-50, station_pos__m(2)+50])
legend('show')

subplot(4,2,4);
hold on; grid on;
plot(station_pos__m(2) - pos(:,2), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,5);
hold on; grid on;
yline(station_pos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,3), 'DisplayName', 'EKF (5 States)')
ylim([station_pos__m(3)-50, station_pos__m(3)+50])
legend('show')

subplot(4,2,6);
hold on; grid on;
plot(station_pos__m(3) - pos(:,3), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(cdtr ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'EKF (5 States)')
legend('show')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(ztd, 'DisplayName', 'EKF (5 States)')
legend('show')

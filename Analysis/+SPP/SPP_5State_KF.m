LoadData;

idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

ctr = 1;

idx_Galileo = cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph);
idx_GPS     = cellfun(@(x)strcmp(x(1), 'G'), S.SvSystem_t_eph);

% KF Initialization
nStates = 5;
F = eye(nStates);
Q = diag([0.01 0.01 0.01 1e-3 1e-9]);
kf = Kalman.Kalman(F, Q);
kf.SetInitialStates([refpos__m'; 0; 0]); % ECEF Position [m]
                                              % Receiver Clock Error * c [m]
                                              % Zenith Troposphere Delay [m]

pos = refpos__m;
cdr = 0;
zdt = 0;
P = eye(nStates);

P_pri  = eye(nStates);
P_post = eye(nStates);
X_pri  = kf.X.';
X_post = kf.X.';

KfIterations = 2;

timestamps = unique(S.ObsToc);
for tt = timestamps.'   
    idx = S.ObsToc == tt;

    kf.Predict();

    X_pri(ctr,:)   = kf.X.';
    P_pri(:,:,ctr) = kf.P;

    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & (idx_Galileo | idx_GPS) & ...
                       (S.Code_1 > 1e3) & (S.Code_5 > 1e3) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);
    code_5_idx = code_idx & (S.Band == 5);
    
    [ia,ib] = ismember(S.SvSystemID_t_eph(code_1_idx), S.SvSystemID_t_eph(code_5_idx));
    tmp = find(code_1_idx);
    code_1_idx(tmp(~ia)) = 0;
    
    for ii = 1:KfIterations
        POS = kf.X(1:3);
        CDTR = kf.X(4);
        ZTD = kf.X(5);
    
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

        cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
        cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);
    
        % elevation mask
        elevation = Generic.CalcElevation(POS(1), POS(2), POS(3), d(:,1), d(:,2), d(:,3));
        
        idx_el = elevation > (15/180*pi) & elevation < (165/180*pi);    
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
             
        pseudorange_est = rho_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset;        
        
        H = [-e, ones(numel(e(:,1)),1), M_E];           
        dy = pseudorange_est - geo_dist;
    
        if ii < KfIterations
            kf.CorrectIterativeEKF(H, dy, eye(numel(dy)));
        else
            kf.CorrectEKF(H, dy, eye(numel(dy)));
        end
    end

    pos(ctr,:) = kf.X(1:3);
    cdr(ctr,:) = kf.X(4);
    ztd(ctr,:) = kf.X(5);    
    P(ctr,:) = diag(kf.P)';

    % for RTS
    X_post(ctr,:) = kf.X.';
    P_post(:,:,ctr) = kf.P;

    ctr = ctr+1;
end
         
% ReadRtkPos;
% return;
%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(refpos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,1), 'DisplayName', 'EKF (5 States)')
ylim([refpos__m(1)-50,  refpos__m(1)+50])
legend('show')

subplot(4,2,2);
hold on; grid on;
plot(refpos__m(1) - pos(:,1), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,3);
hold on; grid on;
yline(refpos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,2), 'DisplayName', 'EKF (5 States)')
ylim([refpos__m(2)-50, refpos__m(2)+50])
legend('show')

subplot(4,2,4);
hold on; grid on;
plot(refpos__m(2) - pos(:,2), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,5);
hold on; grid on;
yline(refpos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos(:,3), 'DisplayName', 'EKF (5 States)')
ylim([refpos__m(3)-50, refpos__m(3)+50])
legend('show')

subplot(4,2,6);
hold on; grid on;
plot(refpos__m(3) - pos(:,3), 'DisplayName', 'EKF (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(cdr ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'EKF (5 States)')
legend('show')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(ztd, 'DisplayName', 'EKF (5 States)')
legend('show')

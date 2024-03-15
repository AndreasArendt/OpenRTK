%% PPP - Single Frequency, Single Constellation, Pseudorange only

% clear;
% LoadData;

%% find number of unique Galileo Satellites
idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

%%
kalman_cfg = struct();
kalman_cfg.idx_pos_N = 1;              % pos N
kalman_cfg.idx_pos_E = 2;              % pos E
kalman_cfg.idx_pos_D = 3;              % pos D
kalman_cfg.idx_cdt   = 4;              % receiver clock bias
kalman_cfg.idx_I     = 5:5+number_E-1; % Ionosphere delay

N = -Inf;
for fn = fieldnames(kalman_cfg).'
    N = max([N, kalman_cfg.(fn{:})]);
end

%%
timestamps = unique(S.ObsToc);
dt = mean(diff(timestamps));

F = zeros(N);
Psi = eye(N) + F * dt;

Q_pos  = 1e-3;
Q_cdt  = 1e-3;
Q_I    = 1e-5;

q([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D]) = Q_pos;
q([kalman_cfg.idx_cdt])                                               = Q_cdt;
q([kalman_cfg.idx_I])                                                 = Q_I;

Q = diag(q);

kalman = Kalman.Kalman(N, Psi, Q);
kalman.SetInitialStates([station_pos__m'; 0; zeros(numel(kalman_cfg.idx_I),1)]);
kalman.SetInitialCovariance(eye(N));

pos    = [];
cdt_rx = [];
Ionosphere   = [];
P = [];

Satellite = struct();
idx_galileo = cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph);

for tt = timestamps.'%(1:180).'   
    fprintf('Processing t %d/%d\n', tt -timestamps(1),  timestamps(end) - timestamps(1));

    idx = S.ObsToc == tt;
  
    kalman.Predict();
    
    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & idx_galileo & ...   % Galileo only!
                       (S.Code_1 > 1e3) & (S.Code_1 > 1e5) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);
    code_5_idx = code_idx & (S.Band == 5);
       
    if sum(code_1_idx) ~= sum(code_5_idx)
        continue;
    end

    %% Pseudorange Correction
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));
    
    cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
    cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);

    tropo_offset = CalcTropoOffset(kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                   d(:,1), d(:,2), d(:,3));

    [Iono_idx] = ismember(SvId_E, S.SvSystemID_t_eph(code_idx));
    Iono = kalman.X(kalman_cfg.idx_I(Iono_idx));

    delta_pseudorange_1 = S.Code_1(code_1_idx) - kalman.X(kalman_cfg.idx_cdt) + cdt_sv - tropo_offset - cdt_rel - Iono;
    delta_pseudorange_5 = S.Code_5(code_5_idx) - kalman.X(kalman_cfg.idx_cdt) + cdt_sv - tropo_offset - cdt_rel - Iono;

    % Pseudorange residual
    v = [ delta_pseudorange_1 - r; ...
          delta_pseudorange_5 - r];

    mu_1 = ones(sum(code_1_idx),1);
    mu_5 = (F_E1_Galileo__Hz / F_E5a_Galileo__Hz) .* ones(sum(code_5_idx),1);

    R = diag(ones(numel(v),1));
        
    I_1 = ones(sum(code_1_idx), 1);
    O_1 = zeros(sum(code_1_idx), 1);
    
    I_5 = ones(sum(code_5_idx), 1);
    O_5 = zeros(sum(code_5_idx), 1);

    MU_1 = diag(mu_1);
    MU_5 = diag(mu_5);
    
    H = [ -e(:,1), -e(:,2), -e(:,3), I_1, MU_1; ...
          -e(:,1), -e(:,2), -e(:,3), I_5, MU_5 ];
    
    kalman.CorrectEKF(H, v, R, 'Index', logical([1; 1; 1; 1; Iono_idx]));

    %%
    pos(end+1,:) = kalman.X([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D]);    
    cdt_rx(end+1) = kalman.X(kalman_cfg.idx_cdt);        
    Ionosphere(end+1,:) = kalman.X(kalman_cfg.idx_I);
    P(end+1,:) = diag(kalman.P)';
end

%%
% t = datetime(timestamps, 'ConvertFrom', 'posixtime');

f = afigure(42);
subplot(4,2,1)
hold on;
plot(pos(:,1));
yline(station_pos__m(1))

subplot(4,2,2)
hold on;
plot(station_pos__m(1) - pos(:,1));

subplot(4,2,3)
hold on;
plot(pos(:,2));
yline(station_pos__m(2))

subplot(4,2,4)
hold on;
plot(station_pos__m(2) - pos(:,2));

subplot(4,2,5)
hold on;
plot(pos(:,3));
yline(station_pos__m(3))

subplot(4,2,6)
hold on;
plot(station_pos__m(3) - pos(:,3));

subplot(4,2,7)
hold on;
plot(cdt_rx ./ Transformation.SpeedOfLight__mDs);

subplot(4,2,8)
hold on;
plot(Ionosphere);


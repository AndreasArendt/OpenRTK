% clear;
% LoadData;

timestamps = unique(S.ObsToc);

%%
kalman_cfg.idx_pos_N = 1;
kalman_cfg.idx_pos_E = 2;
kalman_cfg.idx_pos_D = 3;

kalman_cfg.idx_vel_N = 4;
kalman_cfg.idx_vel_E = 5;
kalman_cfg.idx_vel_D = 6;

kalman_cfg.idx_dt     = 7;  % actually dt * c
kalman_cfg.idx_dt_dot = 8;  % actually dt_dot * c

%%
dt = mean(diff(timestamps));

F = zeros(8);
F([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D], [kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]) = eye(3); % velocity
F(kalman_cfg.idx_dt, kalman_cfg.idx_dt_dot)     = 1;     % delta t dot

Psi = eye(8) + F * dt;

Q_pos  = 1e-3;
Q_vel  = 0; %1e-5; % static configuration
Q_t    = 1e-3;
Q_tdot = 1e-3;

Q = diag([Q_pos Q_pos Q_pos Q_vel Q_vel Q_vel Q_t Q_tdot]);

kalman = Kalman.Kalman(8, Psi, Q);
kalman.SetInitialStates([station_pos__m'; 0; 0; 0; 0; 0]);
kalman.SetInitialCovariance(eye(8));

pos = [];
vel = [];
dt_rx = [];
dt__dot_rx = [];

Satellite = struct();

for tt = timestamps.'    
    idx = S.ObsToc == tt;
  
    kalman.Predict();
    
    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    idx = idx & (S.Snr_1 > 0) & (S.Snr_5 > 0);        

    % Indices
    code_idx    = idx & (S.Code_1 > 1e3 & S.Code_5 > 1e3) & (S.Band == 1);
    phase_idx   = idx & (S.Phase_1 > 0  & S.Phase_5 > 0)  & (S.Band == 1);
    doppler_idx = idx & (S.Doppler_1 ~= 0 & S.Doppler_5 ~= 0);

    %% Doppler Correction
    [~, d, e] = CalcDistance( S.x(doppler_idx), S.y(doppler_idx), S.z(doppler_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    [doppler_est__Hz_L1] = CalcDeltaDoppler( S.xdot(doppler_idx), S.ydot(doppler_idx), S.zdot(doppler_idx), ...
                                          kalman.X(kalman_cfg.idx_vel_N), kalman.X(kalman_cfg.idx_vel_E), kalman.X(kalman_cfg.idx_vel_D), ...
                                          S.SvClockDrift(doppler_idx),  kalman.X(kalman_cfg.idx_dt_dot), e, F_E1_Galileo__Hz);

    [doppler_est__Hz_L5] = CalcDeltaDoppler( S.xdot(doppler_idx), S.ydot(doppler_idx), S.zdot(doppler_idx), ...
                                          kalman.X(kalman_cfg.idx_vel_N), kalman.X(kalman_cfg.idx_vel_E), kalman.X(kalman_cfg.idx_vel_D), ...
                                          S.SvClockDrift(doppler_idx),  kalman.X(kalman_cfg.idx_dt_dot), e, F_E5a_Galileo__Hz);

    v = [ S.Doppler_1(doppler_idx) - doppler_est__Hz_L1; ...
          S.Doppler_5(doppler_idx) - doppler_est__Hz_L5];

    I = ones(numel(e(:,1)), 1);
    O = zeros(numel(e(:,1)), 1);

    H = [ O, O, O, e(:,1), e(:,2), e(:,3), O, I; ...
          O, O, O, e(:,1), e(:,2), e(:,3), O, I];
    R = eye(numel(v)) .* 100;

    % kalman.CorrectEKF(H, v, R);

    %% Zero Velocity Correction
    % H = [ 0, 0, 0, 1, 0, 0, 0, 0; ...
    %       0, 0, 0, 0, 1, 0, 0, 0; ...
    %       0, 0, 0, 0, 0, 1, 0, 0; ];
    % 
    % R = eye(3) ./ 1e3;
    % kalman.CorrectEKF(H, zeros(3,1), R);

    %% Carrierphase Correction
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(phase_idx), S.y(phase_idx), S.z(phase_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    [delta_carrierphase] = CalcDeltaCarrierphase( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                                  S.Phase_1(phase_idx), S.Phase_5(phase_idx), ...
                                                  F_E1_Galileo__Hz, F_E5a_Galileo__Hz, ...
                                                  S.SvClockOffset(phase_idx), kalman.X(kalman_cfg.idx_dt), ...
                                                  d(:,1), d(:,2), d(:,3));

    v = delta_carrierphase - r;

    I = ones(numel(v), 1);
    O = zeros(numel(v), 1);

    H = [ -e(:,1), -e(:,2), -e(:,3), O, O, O, I, O];   
    R = eye(numel(v));

    kalman.CorrectEKF(H, v, R); 

    %% Pseudorange Correction
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(code_idx), S.y(code_idx), S.z(code_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    [delta_pseudorange] = CalcDeltaPseudorange( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                                S.Code_1(code_idx), S.Code_5(code_idx), ...
                                                F_E1_Galileo__Hz, F_E5a_Galileo__Hz, ...
                                                S.SvClockOffset(code_idx), kalman.X(kalman_cfg.idx_dt), S.RelativisticError(code_idx), ...
                                                d(:,1), d(:,2), d(:,3));

    % Pseudorange residual
    v = delta_pseudorange - r;
            
    [~, elevation] = CalcAzimuthElevation( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                           d(:,1), d(:,2), d(:,3));

    R = diag(ones(numel(v),1) .* 1./max(0.1, (elevation-(15/180*pi))).^2);
        
    I = ones(numel(v), 1);
    O = zeros(numel(v), 1);
    H = [ -e(:,1), -e(:,2), -e(:,3), O, O, O, I, O];               
    
    kalman.CorrectEKF(H, v, R);  

    %% Misc Output Data
    [~, d, ~] = CalcDistance( S.x(idx), S.y(idx), S.z(idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));
    [~, elevation] = CalcAzimuthElevation( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                           d(:,1), d(:,2), d(:,3));
    cc = 1;
    for sv = S.SvSystem_t_eph(idx).'
        if isfield(Satellite, sv{:})
            Satellite.(sv{:}).timestamp(end+1) = datetime(tt, 'ConvertFrom', 'posixtime');
            Satellite.(sv{:}).elev(end+1) = elevation(cc);
        else
            Satellite.(sv{:}).timestamp = datetime(tt, 'ConvertFrom', 'posixtime');
            Satellite.(sv{:}).elev = elevation(cc);
        end
        cc = cc + 1;
    end

    %%
    pos(end+1,:) = kalman.X([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D]);
    vel(end+1,:) = kalman.X([kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]);
    dt_rx(end+1) = kalman.X(kalman_cfg.idx_dt);    
    dt__dot_rx(end+1) = kalman.X(kalman_cfg.idx_dt_dot);
end

%%
t = datetime(timestamps, 'ConvertFrom', 'posixtime');

f = afigure(42);
subplot(4,3,1)
hold on;
plot(t, pos(:,1));
yline(station_pos__m(1))

subplot(4,3,2)
hold on;
plot(t, station_pos__m(1) - pos(:,1));

subplot(4,3,3)
hold on;
plot(t, vel(:,1));

subplot(4,3,4)
hold on;
plot(t, pos(:,2));
yline(station_pos__m(2))

subplot(4,3,5)
hold on;
plot(t, station_pos__m(2) - pos(:,2));

subplot(4,3,6)
hold on;
plot(t, vel(:,2));

subplot(4,3,7)
hold on;
plot(t, pos(:,3));
yline(station_pos__m(3))

subplot(4,3,8)
hold on;
plot(t, station_pos__m(3) - pos(:,3));

subplot(4,3,9)
hold on;
plot(t, vel(:,3));

subplot(4,3,10)
hold on;
plot(t, dt_rx ./ Transformation.SpeedOfLight__mDs);

subplot(4,3,11);
hold on;
plot(t, dt__dot_rx ./ Transformation.SpeedOfLight__mDs)

subplot(4,3,12)
hold on;
% axis equal;

for fn = fieldnames(Satellite).'
    plot(Satellite.(fn{:}).timestamp, Satellite.(fn{:}).elev ./ pi .* 180, '.' )
end

% plot(datetime(S.ObsToc, 'ConvertFrom', 'posixtime'), S.SV_numbers, 'x')
% plot(S.ObsToc, S.Snr_1, 'x')
% plot(S.ObsToc, S.Snr_5, 'x')
% plot(pos(:,2), pos(:,1), 'x')
% plot(station_pos__m(2), station_pos__m(1), 'x')

% ax = findobj(get(gcf, 'Children'), 'type', 'axes');
% linkaxes(ax(2:end), 'x')
f.linkaxes('x')




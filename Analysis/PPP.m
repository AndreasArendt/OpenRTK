clear;
LoadData;

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
F = eye(8);
F([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D], [kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]) = 1; % velocity
F(kalman_cfg.idx_dt, kalman_cfg.idx_dt_dot)     = 1;     % delta t dot

Q_pos  = 1e-3;
Q_vel  = 1e-5; % static configuration
Q_t    = 1e-3;
Q_tdot = 1e-3;

Q = diag([Q_pos Q_pos Q_pos Q_vel Q_vel Q_vel Q_t Q_tdot]);

kalman = Kalman.Kalman(8, F, Q);
kalman.SetInitialStates([station_pos__m'; 0; 0; 0; 0; 0]);
timestamps = unique(S.ObsToc);

pos = [];
vel = [];
dt_rx = [];
dt__dot_rx = [];

for tt = timestamps.'    
    idx = S.ObsToc == tt;
  
    kalman.Predict();
    
    % filter out unhealthy satellites
    idx_eph_healthy = S.Health == 0;
    idx = idx & idx_eph_healthy;    
    idx = idx & (S.Band == 1);
        
    % Indices
    code_idx    = idx & (S.Code_1 > 1e3 & S.Code_5 > 1e3);
    phase_idx   = idx & (S.Phase_1 > 0 & S.Phase_5 > 0);
    doppler_idx = idx & (S.Doppler_1 ~= 0);

    %% Doppler Correction
    [~, ~, e] = CalcDistance( S.x(doppler_idx), S.y(doppler_idx), S.z(doppler_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    delta_vel = [ S.xdot(doppler_idx), S.ydot(doppler_idx), S.zdot(doppler_idx)] - ...
                [ kalman.X(kalman_cfg.idx_vel_N), kalman.X(kalman_cfg.idx_vel_E), kalman.X(kalman_cfg.idx_vel_D)];

    delta_clock_drift = kalman.X(kalman_cfg.idx_dt_dot) - Transformation.SpeedOfLight__mDs .* S.SvClockDrift(doppler_idx);

    doppler_est = (-1/(Transformation.SpeedOfLight__mDs / F_E1_Galileo__Hz)) .* (dot(e, delta_vel,2) - delta_clock_drift);

    v = S.Doppler_1(doppler_idx) - doppler_est;
    
    I = ones(numel(v), 1);
    O = zeros(numel(v), 1);

    H = [ O, O, O, e(:,1), e(:,2), e(:,3), O, I];               
    R = eye(numel(v));

    kalman.CorrectEKF(H, v, R);

    %% Pseudorange Correction
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(code_idx), S.y(code_idx), S.z(code_idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    [delta_pseudorange] = CalcDeltaPseudorange( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                                S.Code_1(code_idx), S.Code_5(code_idx), ...
                                                F_E1_Galileo__Hz, F_E5a_Galileo__Hz, ...
                                                S.SvClockOffset(code_idx), kalman.X(kalman_cfg.idx_dt_dot), S.RelativisticError(code_idx), ...
                                                d(:,1), d(:,2), d(:,3));

    % Pseudorange residual
    v = delta_pseudorange - r;
      
    I = ones(numel(v), 1);
    O = zeros(numel(v), 1);

    H = [ -e(:,1), -e(:,2), -e(:,3), O, O, O, I, O];               
    R = eye(numel(v)) .* 100;
    
    kalman.CorrectEKF(H, v, R);

    %%
    pos(end+1,:) = kalman.X([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D]);
    vel(end+1,:) = kalman.X([kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]);
    dt_rx(end+1) = kalman.X(kalman_cfg.idx_dt);    
    dt__dot_rx(end+1) = kalman.X(kalman_cfg.idx_dt_dot);
end

%%
figure(42);
subplot(4,3,1)
hold on;
plot(pos(:,1));
yline(station_pos__m(1))
ylim([station_pos__m(1) - 10, station_pos__m(1) + 10])

subplot(4,3,2)
hold on;
plot(station_pos__m(1) - pos(:,1));

subplot(4,3,3)
hold on;
plot(vel(:,1));

subplot(4,3,4)
hold on;
plot(pos(:,2));
yline(station_pos__m(2))

subplot(4,3,5)
hold on;
plot(station_pos__m(2) - pos(:,2));

subplot(4,3,6)
hold on;
plot(vel(:,2));

subplot(4,3,7)
hold on;
plot(pos(:,3));
yline(station_pos__m(3))

subplot(4,3,8)
hold on;
plot(station_pos__m(3) - pos(:,3));

subplot(4,3,9)
hold on;
plot(vel(:,3));

subplot(4,3,10)
hold on;
plot(dt_rx ./ Transformation.SpeedOfLight__mDs);

subplot(4,3,11);
hold on;
plot(dt__dot_rx ./ Transformation.SpeedOfLight__mDs)

subplot(4,3,12)
hold on;
axis equal;
plot(pos(:,2), pos(:,1), 'x')
plot(station_pos__m(2), station_pos__m(1), 'x')




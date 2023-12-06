clear;
LoadData;

%%
kalman_cfg.idx_pos_N = 1;
kalman_cfg.idx_pos_E = 2;
kalman_cfg.idx_pos_D = 3;

kalman_cfg.idx_vel_N = 4;
kalman_cfg.idx_vel_E = 5;
kalman_cfg.idx_vel_D = 6;

kalman_cfg.idx_dt = 7;      % actually dt * c
kalman_cfg.idx_dt_dot = 8;  % actually dt_dot * c

%%
F = eye(8);
F([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D], [kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]) = 1; % velocity
F(kalman_cfg.idx_dt, kalman_cfg.idx_dt_dot)     = 1;     % delta t dot

Q = diag([0.0001 0.0001 0.0001 0.001 0.001 0.001 0.1 0.001]);

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
    idx = idx & (S.Code_1 > 1e3 & S.Code_5 > 1e3);
    idx = idx & (S.Band == 1);
        
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(idx), S.y(idx), S.z(idx), ...
                              kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D));

    % Correction
    [delta_pseudorange] = CalcDeltaPseudorange( kalman.X(kalman_cfg.idx_pos_N), kalman.X(kalman_cfg.idx_pos_E), kalman.X(kalman_cfg.idx_pos_D), ...
                                                S.Code_1(idx), S.Code_5(idx), ...
                                                F_E1_Galileo__MHz, F_E5a_Galileo__MHz, ...
                                                S.SvClockOffset(idx), kalman.X(kalman_cfg.idx_dt_dot), S.RelativisticError(idx), ...
                                                d(:,1), d(:,2), d(:,3));
      
    % Pseudorange residual
    v = delta_pseudorange - r;
        
    I = ones(numel(v), 1);
    O = zeros(numel(v), 1);

    H = [ -e(:,1), -e(:,2), -e(:,3), O, O, O, I, O];               
    R = eye(numel(v)) .* 100;
    
    kalman.CorrectEKF(H, v, R);

    pos(end+1,:) = kalman.X([kalman_cfg.idx_pos_N, kalman_cfg.idx_pos_E, kalman_cfg.idx_pos_D]);
    vel(end+1,:) = kalman.X([kalman_cfg.idx_vel_N, kalman_cfg.idx_vel_E, kalman_cfg.idx_vel_D]);
    dt_rx(end+1) = kalman.X(kalman_cfg.idx_dt);    
    dt__dot_rx(end+1) = kalman.X(kalman_cfg.idx_dt_dot);
end

%%
figure(42);
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
plot(dt_rx ./ Transformation.SpeedOfLight__mDs);




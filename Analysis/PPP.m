clear;
LoadData;

%%
F = eye(4);
% F(1:3,4:6) = 1; % velocity
% F(7,8) = 1;     % delta t dot

Q = diag([0.001 0.001 0.001 0.0001]);

kalman = Kalman.Kalman(4, F, Q);
kalman.SetInitialStates([station_pos__m'; 0]);

timestamps = unique(S.ObsToc);

pos = [];
dt_rx = [];

for tt = timestamps.'    
    idx = S.ObsToc == tt;
  
    kalman.Predict();
    
    % filter out unhealthy satellites
    idx_eph_healthy = S.Health == 0;
    idx = idx & idx_eph_healthy;
    idx = idx & (S.Code_1 > 1e3 & S.Code_5 > 1e3);
    idx = idx & (S.Band == 1);
        
    % calc distance and distance vector
    [r, d, e] = CalcDistance( S.x(idx), S.y(idx), S.z(idx), kalman.X(1), kalman.X(2), kalman.X(3));

    % Prefit residuals  
    [delta_pseudorange] = CalcDeltaPseudorange( kalman.X(1), kalman.X(2), kalman.X(3), ...
                                                S.Code_1(idx), S.Code_5(idx), ...
                                                F_E1_Galileo__MHz, F_E5a_Galileo__MHz, ...
                                                S.SvClockOffset(idx), kalman.X(4), S.RelativisticError(idx), ...
                                                d(:,1), d(:,2), d(:,3));
      
    v = delta_pseudorange - r;
        
    C = Transformation.SpeedOfLight__mDs .* ones(numel(v), 1);
    
    H = [ -e(:,1), -e(:,2), -e(:,3), ones(numel(v), 1)];               
    R = eye(numel(v)) .* 100;
    
    kalman.CorrectEKF(H, v, R);

    pos(end+1,:) = kalman.X(1:3);
    dt_rx(end+1) = kalman.X(4);
end

%%
figure
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




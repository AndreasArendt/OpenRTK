clear;

eph = table2struct(readtable("Analysis\Ephemeris_exp.csv"), 'ToScalar',true);
obs = table2struct(readtable("Analysis\Observation_exp.csv"), 'ToScalar',true);

eph.Time_Sv_Tuple = strcat(string(eph.ObsToc), "_", string(eph.SvSystem));
obs.Time_Sv_Tuple = strcat(string(obs.UTC), "_", string(obs.SvSystem));

t_eph = struct2table(eph);
t_obs = struct2table(obs);

T = join(t_eph, t_obs, 'Keys',{'Time_Sv_Tuple'}');
S = table2struct(T, 'ToScalar', true);
S.SV_numbers = cellfun(@(x)(str2num(x(2:end))), S.SvSystem_t_eph);

station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
% station_pos__m = [4498451.8100  1708266.8300  4173591.7800];
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

F_E1_Galileo__MHz  = 1575.420;
F_E5a_Galileo__MHz = 1176.450;

clear eph;
clear obs;

%%
c__mDs = 299792458;

ctr = 1;

F = eye(4);
X(:,ctr) = [station_pos__m'; 0];
P(:,:,ctr) = zeros(4,4);
Q = diag([0.001 0.001 0.001 0.0001]);
y(ctr) = 0;
R = 100;

timestamps = unique(S.ObsToc);

for tt = timestamps.'    
    idx = S.ObsToc == tt;
        
    % Prediction
    X(:,ctr+1)   = F * X(:,ctr);
    P(:,:,ctr+1) = F * P(:,:,ctr) * F.' + Q;
    
    % Correction       
    
    % filter out unhealthy satellites
    idx_eph_healthy = S.Health == 0;
    idx = idx & idx_eph_healthy;
    idx = idx & (S.Code_1 > 1e3 & S.Code_5 > 1e3);
             
    % Prefit residuals  
    dx = (S.x(idx) - X(1,ctr+1));
    dy = (S.y(idx) - X(2,ctr+1));
    dz = (S.z(idx) - X(3,ctr+1));
    r = sqrt( dx.^2 + dy.^2 + dz.^2);
    
    % Sagnac Effect
    r = r + Transformation.MeanAngularVelocityOfEarth__radDs*(S.x(idx)*X(2,ctr+1) - S.y(idx)*X(1,ctr+1)) ./ Transformation.SpeedOfLight__mDs;

    cdt_sv = c__mDs * S.SvClockOffset(idx);
    cdt_rx = c__mDs * X(4,ctr+1);
    cdt_relativistic = c__mDs * S.RelativisticError(idx);

    [lat__rad, lon__rad, alt__m] = Transformation.ecef2wgs84(X(1,ctr+1), X(2,ctr+1), X(3,ctr+1));                
    [E,N,U] = Transformation.ecef2enu(lat__rad, lon__rad, dx, dy, dz);
    
    elevation = atan2(U, sqrt(E.^2 + N.^2));

    dt_tropo = CalcTropoDelay(alt__m, elevation);
            
    r_ionofree = ((F_E1_Galileo__MHz^2 * S.Code_1(idx)) - (F_E5a_Galileo__MHz^2 * S.Code_5(idx))) ./ ...
                        (F_E1_Galileo__MHz^2 - F_E5a_Galileo__MHz^2);
    
    H1 = (X(1,ctr+1) - S.x(idx)) ./ r; 
    H2 = (X(2,ctr+1) - S.y(idx)) ./ r; 
    H3 = (X(3,ctr+1) - S.z(idx)) ./ r; 

    C = repmat(c__mDs, numel(r),1);
    H = [H1, H2, H3, C];
     
    dist_meas = r_ionofree - cdt_rx + cdt_sv - dt_tropo - cdt_relativistic;        
    y = dist_meas - r;
   
    DOP(ctr) = norm(diag(inv([H1, H2, H3, ones(numel(r),1)].'*[H1, H2, H3, ones(numel(r),1)])));
    
    RR = eye(sum(idx)) .* R;

    K = P(:,:,ctr+1) * H' / (H * P(:,:,ctr+1) * H.' + RR);
    X(:,ctr+1) = X(:,ctr+1) + K * y;
    P(:,:,ctr+1) = (eye(4) - K*H) * P(:,:,ctr+1) * (eye(4) - K*H).' + K * RR * K.';

    ctr = ctr+1;    
end

%%
figure; 
ax(1) = subplot(4,2,1);
title('x_E')
hold on;
plot(timestamps, X(1,2:end)); 
yline(station_pos__m(1))

ax(2) = subplot(4,2,2);
title('delta x_E')
hold on;
plot(timestamps(2:end), station_pos__m(1) - X(1,3:end)); 

ax(3) = subplot(4,2,3);
title('y_E')
hold on;
plot(timestamps, X(2,2:end));
yline(station_pos__m(2))

ax(4) = subplot(4,2,4);
title('delta y_E')
hold on;
plot(timestamps(2:end), station_pos__m(2) - X(2,3:end)); 

ax(5) = subplot(4,2,5);
title('z_E')
hold on;
plot(timestamps(2:end), X(3,3:end)); 
yline(station_pos__m(3))

ax(6) = subplot(4,2,6);
title('dt_r')
hold on;
plot(timestamps(2:end), X(4,3:end)); 

ax(7) = subplot(4,2,7);
title('GDOP')
plot(timestamps, DOP); 

ax(8) = subplot(4,2,8);
title('Health')
hold on;
plot(S.ObsToc(S.Health==0), S.SV_numbers(S.Health==0), 'x', 'Color', Color.GREEN)
plot(S.ObsToc(S.Health~=0), S.SV_numbers(S.Health~=0), 'x', 'Color', Color.RED)

linkaxes(ax, 'x');

%%

idx = S.SV_numbers == 3;

dx = (S.x- station_pos__m(1));
dy = (S.y - station_pos__m(2));
dz = (S.z - station_pos__m(3));
rr = sqrt( dx.^2 + dy.^2 + dz.^2);

figure;
axx(1) = subplot(3,1,1);
hold on;
plot(S.ObsToc(idx) - min(S.ObsToc(idx)), S.Code_1(idx), '.')
plot(S.ObsToc(idx) - min(S.ObsToc(idx)), rr(idx), '.')

axx(2) = subplot(3,1,2);
% plot(timestamps(2:end) - timestamps(2), X(4,3:end)); 
plot(S.ObsToc(idx) - min(S.ObsToc(idx)), S.Code_1(idx) - rr(idx), '.')

axx(3) = subplot(3,1,3);
plot(S.ObsToc(idx) - min(S.ObsToc(idx)), S.SvClockOffset(idx), '.')

linkaxes(axx, 'x')






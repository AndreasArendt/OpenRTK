clear; close all;

% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000];
station_pos__m = [4498451.8100  1708266.8300  4173591.7800];
                  
eph = table2struct(readtable("Analysis\Ephemeris_exp.csv"), 'ToScalar',true);
obs = table2struct(readtable("Analysis\Observation_exp.csv"), 'ToScalar',true);

%% Plotting
figure;
axis equal; hold on; grid on;
[x,y,z] = ellipsoid(0,0,0,6378137.000,6378137.000,6356752.314);
surf(x,y,z);

plot3(station_pos__m(1), station_pos__m(2), station_pos__m(3), 'x');

for sv = unique(eph.SvSystem).'
    idx_eph = strcmp(eph.SvSystem, sv);
    idx_obs = strcmp(obs.SvSystem, sv);

    plot3(eph.x(idx_eph), eph.y(idx_eph), eph.z(idx_eph), '-x');
    % plot3(eph.x(idx), eph.y(idx), eph.z(idx), 'x');
end

%%
sv = 'E24';
idx_eph = strcmp(eph.SvSystem, sv);
idx_obs = strcmp(obs.SvSystem, sv);

delta_pos_E = [eph.x(idx_eph), eph.y(idx_eph), eph.z(idx_eph)] - station_pos__m;
dist = sqrt(delta_pos_E(:,1).^2 + delta_pos_E(:,2).^2 + delta_pos_E(:,3).^2);
dist_interp = interp1(eph.UTC(idx_eph), dist, obs.UTC(idx_obs), 'linear', 'extrap');
dist_interp2 = interp1(eph.ObsToc(idx_eph), dist, obs.UTC(idx_obs), 'linear', 'extrap');

figure;
ax(1) = subplot(2,1,1)
hold on;
plot(eph.ObsToc(idx_eph), dist, 'DisplayName', 'dist')
plot(eph.UTC(idx_eph), dist, 'DisplayName', 'dist 2')
plot(obs.UTC(idx_obs), obs.Code_1(idx_obs), 'x', 'DisplayName', 'Code 1')
% plot(obs.UTC(idx_obs), obs.Code_5(idx_obs), 'DisplayName', 'Code 5')
legend('show')


ax(2) = subplot(2,1,2)
hold on;
plot(obs.UTC(idx_obs), (dist_interp - obs.Code_1(idx_obs)) ./ 1e3)
plot(obs.UTC(idx_obs), (dist_interp2 - obs.Code_1(idx_obs)) ./ 1e3)


linkaxes(ax, 'x')
%%

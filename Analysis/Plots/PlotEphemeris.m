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



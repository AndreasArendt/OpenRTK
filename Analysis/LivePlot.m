clear; close all;

station_pos__m = [4164313.0000, 803525.9000, 4748474.9000];

eph = table2struct(readtable("Analysis\Ephemeris_exp.csv"), 'ToScalar',true);
obs = table2struct(readtable("Analysis\Observation_exp.csv"), 'ToScalar',true);

figure;
axis equal; hold on; grid on;
view(90,45);
[x,y,z] = ellipsoid(0,0,0,6378137.000,6378137.000,6356752.314);
surf(x,y,z);

plot3(station_pos__m(1), station_pos__m(2), station_pos__m(3), 'x');

for ii = unique(eph.UTC.')
    idx = eph.UTC == ii;

    plot3(eph.x(idx), eph.y(idx), eph.z(idx), 'x');

    drawnow;
    pause(0.1);
end
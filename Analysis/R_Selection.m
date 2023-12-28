elevation = 0:0.001:pi/2;

figure;
hold on;
plot(elevation ./ pi .* 180, 1./max(0.1, (elevation-(15/180*pi))).^2)
plot(elevation ./ pi .* 180, 1./sin(max(0.1, (elevation-(15/180*pi)))).^2)
plot(elevation ./ pi .* 180, 1./sin(elevation))
posfile = 'D:\Projekte\OpenRTK\data\sampledata\obs\ORID00MKD_R_20230090000_01D_30S_MO.pos';

s = table2struct(readtable(posfile, 'NumHeaderLines',17, 'FileType','text'), 'ToScalar',true);

lat = s.Var3;
lon = s.Var4;
alt = s.Var5;


% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

[x_E, y_E, z_E] = Transformation.wgs84ToEcef(lat ./ 180 .* pi, lon ./ 180 .* pi, alt);
x_N = x_E - station_pos__m(1);
y_N = y_E - station_pos__m(2);
z_N = z_E - station_pos__m(3);

%%
figure;
subplot(3,2,1)
hold on;
plot( x_E, 'DisplayName', 'RTKLIB')
yline( station_pos__m(1))
plot(pos(:,1), 'DisplayName', 'KF'); 
legend('show')

subplot(3,2,2)
hold on;
plot( station_pos__m(1) - x_E, 'DisplayName', 'RTKLIB')
plot( station_pos__m(1) - pos(:,1), 'DisplayName', 'KF'); 
legend('show')

subplot(3,2,3)
hold on;
plot( y_E, 'DisplayName', 'RTKLIB')
yline( station_pos__m(2))
plot(pos(:,2), 'DisplayName', 'KF'); 
legend('show')

subplot(3,2,4)
hold on;
plot( station_pos__m(2) - y_E, 'DisplayName', 'RTKLIB')
plot( station_pos__m(2) - pos(:,2), 'DisplayName', 'KF'); 
legend('show')

subplot(3,2,5)
hold on;
plot( z_E, 'DisplayName', 'RTKLIB')
yline( station_pos__m(3))
plot(pos(:,3), 'DisplayName', 'KF'); 
legend('show')

subplot(3,2,6)
hold on;
plot( station_pos__m(3) - z_E, 'DisplayName', 'RTKLIB')
plot( station_pos__m(3) - pos(:,3), 'DisplayName', 'KF'); 
legend('show')
posfile = 'D:\Projekte\OpenRTK\data\sampledata\obs\ORID00MKD_R_20230090000_01D_30S_MO.pos';

s = table2struct(readtable(posfile, 'NumHeaderLines',17, 'FileType','text'), 'ToScalar',true);

lat = s.Var3;
lon = s.Var4;
alt = s.Var5;

[x_E, y_E, z_E] = Transformation.wgs84ToEcef(lat ./ 180 .* pi, lon ./ 180 .* pi, alt);

% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

%%
figure;
subplot(3,1,1)
hold on;
plot( x_E)
yline( station_pos__m(1))
plot(X(1,2:end)); 

subplot(3,1,2)
hold on;
plot( y_E)
yline( station_pos__m(2))
plot(X(2,2:end)); 

subplot(3,1,3)
hold on;
plot( z_E)
yline( station_pos__m(3))
plot(X(3,2:end)); 
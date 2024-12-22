clear;
LoadData();

%% settings
sv = 'G9';

%% Prepare Precise Emeperis
peph = NaN(numel(PreciseEphemerisData), 3);
for ii = 1:numel(PreciseEphemerisData)
    idx = strcmp({PreciseEphemerisData(ii).Ephemeris.SatelliteSystem}, sv);

    pEph = PreciseEphemerisData(ii).Ephemeris(idx).Position_E;
    peph(ii,:) = [pEph.x, pEph.y, pEph.z];
end

t_peph = datetime([PreciseEphemerisData.PosixEpochTime__s], 'ConvertFrom', 'posix').';

%% Prepare Broadcast Ephemeris
beph = NaN(numel(SatelliteData), 3);
for ii = 1:numel(SatelliteData)
    idx = strcmp({SatelliteData(ii).Observations.SatelliteSystem}, sv);

    bEph = SatelliteData(ii).Observations(idx).ECEF_Position;
    beph(ii,:) = [bEph.x, bEph.y, bEph.z];
end

t_beph = datetime([SatelliteData.PosixEpochTime__s], 'ConvertFrom', 'posix').';

%% delta
delta_x = peph(:,1) - interp1(t_beph, beph(:,1), t_peph);
delta_y = peph(:,2) - interp1(t_beph, beph(:,2), t_peph);
delta_z = peph(:,3) - interp1(t_beph, beph(:,3), t_peph);

%%
af = afigure(1208);
subplot(3,2,1)
hold on; grid on;
plot(t_beph, beph(:,1), 'DisplayName', 'Broadcast x_E')
plot(t_peph, peph(:,1), 'x', 'DisplayName', 'Precise x_E')
legend('show')

subplot(3,2,2)
hold on; grid on;
plot(t_peph, delta_x, 'x-', 'DisplayName', 'delta-x')
legend('show')

subplot(3,2,3)
hold on; grid on;
plot(t_beph, beph(:,2), 'DisplayName', 'Broadcast y_E')
plot(t_peph, peph(:,2), 'x', 'DisplayName', 'Precise y_E')
legend('show')

subplot(3,2,4)
hold on; grid on;
plot(t_peph, delta_y, 'x-', 'DisplayName', 'delta-y')
legend('show')

subplot(3,2,5)
hold on; grid on;
plot(t_beph, beph(:,3), 'DisplayName', 'Broadcast z_E')
plot(t_peph, peph(:,3), 'x', 'DisplayName', 'Precise z_E')
legend('show')

subplot(3,2,6)
hold on; grid on;
plot(t_peph, delta_z, 'x-', 'DisplayName', 'delta-z')
legend('show')

af.linkaxes('x');
xlim([t_beph(1) - 0.001, t_beph(end) + 0.001])
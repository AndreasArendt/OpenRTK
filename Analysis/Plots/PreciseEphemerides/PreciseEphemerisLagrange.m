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

%%
% new 5Hz timestamp
t_new = t_beph;%[t_peph(1):seconds(0.2):t_peph(1)+hours(2)].';

%%
af = afigure;
subplot(4,1,1);
hold on; grid on;

n_poly = [3 5 7 9];

x_interp = NaN(numel(t_new), max(n_poly));
for n = n_poly
    % number of samples on the left half of number of polys
    left = floor(n/2);

    % find first timestamp that is larger than our polynomial
    st = find(t_new > t_peph(left),1);

    id_en = find(t_peph > t_new(end), 1);
    en = find(t_new < t_peph(id_en-left),1 ,'last');

    for ii = st:en

        % find closest n_lagrange indices
        [~, sortedIndices] = sort(abs(t_peph-t_new(ii)));
        idx = sort(sortedIndices(1:n));

        x = t_peph(idx);
        y = peph(idx,1);
        x_interp(ii, n) = lagrange_interp(x, y, t_new(ii));
    end

    plot(t_new, x_interp(:,n), '.-', 'DisplayName', ['Lagrange - ' num2str(n)])
end

plot(t_peph, peph(:,1), 'x', 'DisplayName', 'Precise Eph')
plot(t_beph, beph(:,1), '.', 'DisplayName', 'Broadcast Eph');

subplot(4,1,2);
hold on; grid on;
plot(t_new, x_interp(:,9) - x_interp(:,7), 'DisplayName', '9-7')
plot(t_new, x_interp(:,9) - x_interp(:,5), 'DisplayName', '9-5')
plot(t_new, x_interp(:,9) - x_interp(:,3), 'DisplayName', '9-3')

subplot(4,1,3);
hold on; grid on;
plot(t_new, x_interp(:,7) - x_interp(:,5), 'DisplayName', '7-5')
plot(t_new, x_interp(:,7) - x_interp(:,3), 'DisplayName', '5-3')

subplot(4,1,4);
hold on; grid on;
plot(t_new, x_interp(:,9) - beph(:,1), 'DisplayName', '9 - beph')
plot(t_new, x_interp(:,7) - beph(:,1), 'DisplayName', '7 - beph')
plot(t_new, x_interp(:,5) - beph(:,1), 'DisplayName', '5 - beph')
plot(t_new, x_interp(:,3) - beph(:,1), 'DisplayName', '3 - beph')

af.showlegend();
af.linkaxes('x');
xlim([t_new(1) t_new(end)])
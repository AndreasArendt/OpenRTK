clear;
LoadData();

%% settings
sv = 'G9';

%% Prepare Precise Clock
pclk__s = NaN(numel(PreciseClockData), 1);
for ii = 1:numel(PreciseClockData)
    idx = strcmp({PreciseClockData(ii).Clock.SatelliteSystem}, sv);

    pclk__s(ii) = PreciseClockData(ii).Clock(idx).SatelliteClockError__s;    
end

t_pclk = datetime([PreciseClockData.PosixEpochTime__s], 'ConvertFrom', 'posix').';

%% Prepare Broadcast Clock
bclk__s = NaN(numel(SatelliteData), 1);
for ii = 1:numel(SatelliteData)
    idx = strcmp({SatelliteData(ii).Observations.SatelliteSystem}, sv);
    
    bclk__s(ii,:) = SatelliteData(ii).Observations(idx).ClockOffset;
end

t_bclk = datetime([SatelliteData.PosixEpochTime__s], 'ConvertFrom', 'posix').';

%% delta
delta_clock = pclk__s - interp1(t_bclk, bclk__s, t_pclk, 'nearest');

%%
af = afigure(42);
subplot(2,1,1);
hold on; grid on;
plot(t_pclk, pclk__s, 'DisplayName', 'Precise Clock');
plot(t_bclk, bclk__s, 'DisplayName', 'Broadcast Clock');

subplot(2,1,2);
hold on; grid on;
plot(t_pclk, delta_clock, 'DisplayName', 'delta');

af.linkaxes('x')
af.showlegend();
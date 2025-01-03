function DictPclk = GetPreciseClock(PreciseClock, Satellite, time__s)
%G Summary of this function goes here
%   Detailed explanation goes here

arguments (Input)
    PreciseClock
    Satellite
    time__s
end

arguments (Output)
    DictPclk
end

% get indices of ephemerides data
idx_st = find([PreciseClock.PosixEpochTime__s] > time__s, 1) - 1;
idx_en = min(idx_st + 1, numel(PreciseClock));

assert(idx_en > idx_st, 'Cannot Interpolate outside of given data range'); % TODO make extrap option

% filter Ephemerides data and get timestamps
ClockData = [PreciseClock(idx_st:idx_en).Clock];
lia = ismember({ClockData.SatelliteSystem}, Satellite);

filtered_ClockData = ClockData(lia);
t_clk = [PreciseClock(idx_st:idx_en).PosixEpochTime__s];

% iterate over all satellites and interpolate data - add to dictionary
DictPclk = dictionary();
for sv = Satellite(:).'
    idx = ismember({filtered_ClockData.SatelliteSystem}, sv);
    
    sv_clk_error__s = [filtered_ClockData(idx).SatelliteClockError__s];    
    sv_clk_error__s_interp = interp1(t_clk, sv_clk_error__s, time__s, 'linear');
    
    DictPclk = DictPclk.insert(sv{:}, sv_clk_error__s_interp);    
end
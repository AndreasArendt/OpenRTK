function DictPeph = getPreciseEphemeris(PreciseEphemeris, Satellite, time__s, options)
%G Summary of this function goes here
%   Detailed explanation goes here

arguments (Input)
    PreciseEphemeris
    Satellite
    time__s
    options.n_polynomials = 9
end

arguments (Output)
    DictPeph
end

% this is number of samples (left/right) for lagrange interpolation
interp_len = floor(options.n_polynomials/2);

% get update rate of precise emphemerides data
dt__s = mean(diff([PreciseEphemeris.PosixEpochTime__s]));

% use time window to pre-filter ephemerides data
time_window = interp_len * dt__s;

% get indices of ephemerides data
idx_st = find([PreciseEphemeris.PosixEpochTime__s] > time__s - time_window,1);
idx_en = find([PreciseEphemeris.PosixEpochTime__s] > time__s + time_window,1);

% filter Ephemerides data and get timestamps
EphemerisData = [PreciseEphemeris(idx_st:idx_en).Ephemeris];
lia = ismember({EphemerisData.SatelliteSystem}, Satellite);

filtered_EphemerisData = EphemerisData(lia);
t_eph = [PreciseEphemeris(idx_st:idx_en).PosixEpochTime__s];

% iterate over all satellites and interpolate data - add to dictionary
DictPeph = dictionary();
for sv = Satellite(:).'
    idx = ismember({filtered_EphemerisData.SatelliteSystem}, sv);
    
    position_E = [filtered_EphemerisData(idx).Position_E];
    
    pos_E.x = lagrange_interp(t_eph, [position_E.x], time__s);
    pos_E.y = lagrange_interp(t_eph, [position_E.y], time__s);
    pos_E.z = lagrange_interp(t_eph, [position_E.z], time__s);    

    DictPeph = DictPeph.insert(sv{:}, pos_E);    
end
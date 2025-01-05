function Observations = getValidObservations(Observations, options)

arguments
    Observations 
    options.excludeSv = {''} 
end

    %%
    idx_gps = startsWith({Observations.SatelliteSystem}, 'G');
    idx_gal = startsWith({Observations.SatelliteSystem}, 'E');
    
    Code    = [Observations.Code];
    Carrier = [Observations.Carrier];

    %%
    idx_code_1 = [Code.Band_1] > 0;
    idx_code_2 = [Code.Band_2] > 0;

    idx_code = idx_code_1 & idx_code_2;

    %%
    idx_carrier_1 = [Carrier.Band_1] > 0;
    idx_carrier_2 = [Carrier.Band_2] > 0;

    idx_carrier = idx_carrier_1 & idx_carrier_2;

    %%
    idx_healthy = [Observations.IsHealthy];

    % exclude satellites
    [~, ib] = ismember(options.excludeSv, {Observations.SatelliteSystem});
    idx_exclude = true(1, numel(Observations));
    if any(ib)
        idx_exclude(ib) = false;
    end

    idx = idx_gps & idx_code & idx_carrier & idx_healthy & idx_exclude;
    
    Observations = Observations(idx);
end
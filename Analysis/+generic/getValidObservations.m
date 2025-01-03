function Observations = getValidObservations(Observations)

    idx_gps = startsWith({Observations.SatelliteSystem}, 'G');
    idx_gal = startsWith({Observations.SatelliteSystem}, 'E');
    
    Code    = [Observations.Code];

    idx_code_1 = [Code.Band_1] > 0;
    idx_code_2 = [Code.Band_2] > 0;
    idx_code_5 = [Code.Band_5] > 0;

    idx_healthy = [Observations.IsHealthy];

    idx = idx_gps & idx_code_1 & idx_code_2 & idx_healthy;
    
    Observations = Observations(idx);
end
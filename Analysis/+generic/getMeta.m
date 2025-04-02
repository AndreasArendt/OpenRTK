function meta = getMeta(SatelliteData)

%% get healthy satellites
svs = {};
for ii = 1:numel(SatelliteData)       
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);
    svs = unique([svs(:)', {Observations.SatelliteSystem}]);
end

meta.NumberSatellites          = numel(svs);
meta.AvailableSatelliteSystems = svs;

end
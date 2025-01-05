function meta = getMeta(SatelliteData)

%% get healthy satellites
svs = {};
for ii = 1:numel(SatelliteData)   
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);
    svs = unique([svs(:)', {Observations.SatelliteSystem}]);
end

% create struct with empty sv health vector
% s = struct();
% for sv = svs
%     s.(sv{:}) = nan(numel(SatelliteData),1);
% end

% fill health vector
% for ii = 1:numel(SatelliteData)   
%     Observations = generic.getValidObservations(SatelliteData(ii).Observations);
% 
%     for fn = fieldnames(s).'
%         if ismember(fn{:}, {Observations.SatelliteSystem})
%             s.(fn{:})(ii) = 1;
%         else
%             s.(fn{:})(ii) = 0;
%         end
%     end
% end

meta.NumberSatellites          = numel(svs);
meta.AvailableSatelliteSystems = svs;

end
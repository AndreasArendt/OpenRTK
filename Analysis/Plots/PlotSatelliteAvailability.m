% get all healthy satellites
svs = {};
for ii = 1:numel(SatelliteData)   
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);
    svs = unique([svs(:)', {Observations.SatelliteSystem}]);
end

% create struct with empty sv availability vector
s = struct();
for sv = svs
    s.(sv{:}) = nan(numel(SatelliteData),1);
end

% fill availability vector
for ii = 1:numel(SatelliteData)   
    Observations = generic.getValidObservations(SatelliteData(ii).Observations);

    for fn = fieldnames(s).'
        if ismember(fn{:}, {Observations.SatelliteSystem})
            s.(fn{:})(ii) = 1;
        else
            s.(fn{:})(ii) = 0;
        end
    end
end

%%
ff = afigure;
hold on; grid on;

fns = fieldnames(s);
for ii = 1:numel(fns)
    plot(s.(fns{ii}) * 0.5 + ii - 0.5)
end

yticklabels(fns)
for ii = 1:numel(SatelliteData)
    [SatelliteData(ii).Observations.PosixEpochTime__s] = deal(SatelliteData(ii).PosixEpochTime__s);
end

obs = vertcat(SatelliteData.Observations);
time = horzcat(SatelliteData.PosixEpochTime__s);
svs = unique({obs.SatelliteSystem});

satdata = struct();

for sv = svs
    idx = cellfun(@(x) strcmp(x, sv{:}),{obs.SatelliteSystem});
    satdata.(sv{:}) = table2struct(struct2table(obs(idx)), 'ToScalar', true);

    for fn = fieldnames(satdata.(sv{:})).'
        tmp = satdata.(sv{:}).(fn{:});
        
        if isstruct(tmp)
            satdata.(sv{:}).(fn{:}) = table2struct(struct2table(tmp),"ToScalar",true);
        end        
    end

    satdata.(sv{:}).timestamp = satdata.(sv{:}).PosixEpochTime__s - satdata.(sv{:}).PosixEpochTime__s(1);
end
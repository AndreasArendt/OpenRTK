% LoadData;

obs = vertcat(SatelliteData.Observations);
svs = unique({obs.SatelliteSystem});

svs = svs(startsWith(svs, 'G'));

af = afigure(42);
hold on; grid on;

for ii = 1:numel(SatelliteData)
    %% Pre-Processing
    Observations = SatelliteData(ii).Observations;

    idx_gps = startsWith({Observations.SatelliteSystem}, 'G17');
    idx_gal = startsWith({Observations.SatelliteSystem}, 'E');
    
    Code    = [Observations.Code];

    idx_code_1 = [Code.Band_1] > 0;
    idx_code_2 = [Code.Band_2] > 0;
    idx_code_5 = [Code.Band_5] > 0;

    idx_healthy = [Observations.IsHealthy];

    idx = idx_gps & idx_code_1 & idx_code_2 & idx_healthy;
    
    % TODO: SV order is implicitly used here

    if (SatelliteData(ii).PosixEpochTime__s - SatelliteData(1).PosixEpochTime__s) >= 191.4
        a = 1;
    end

    d = Generic.GetPreciseEphemeris(PreciseEphemerisData, {Observations(idx).SatelliteSystem}, SatelliteData(ii).PosixEpochTime__s);    
    v = [d.entries('Struct').Value];

    if ~isempty(v)
        ECEF_x = [v.x].';
        ECEF_y = [v.y].';
        ECEF_z = [v.z].';
        
        plot(SatelliteData(ii).PosixEpochTime__s - SatelliteData(1).PosixEpochTime__s, ECEF_x, 'x', 'Color', Color.BLUE);
    end
end

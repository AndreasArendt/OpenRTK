clear;
LoadData();

%% settings
sv = 'G9';

%% Prepare Precise Emeperis
peph = NaN(numel(PreciseEphemerisData), 3);
for ii = 1:numel(PreciseEphemerisData)
    idx = strcmp({PreciseEphemerisData(ii).Ephemeris.SatelliteSystem}, sv);

    pEph = PreciseEphemerisData(ii).Ephemeris(idx).Position_E;
    peph(ii,:) = [pEph.x, pEph.y, pEph.z];
end

t_peph = datetime([PreciseEphemerisData.PosixEpochTime__s], 'ConvertFrom', 'posix').';

%%
% new 5Hz timestamp
% t_new = [t_peph(1):milliseconds(200):t_peph(1)+hours(1)].';
t_new = [t_peph(1):seconds(10):t_peph(1)+hours(2)].';

%%
af = afigure;
hold on; grid on;
for n = 2:1:9

    n_lagrange_polynomial =n;
    
    % number of samples on the left half of number of polys
    left = floor(n_lagrange_polynomial/2); 
    
    % find first timestamp that is larger than our polynomial
    st = find(t_new > t_peph(left),1);
    en = find(t_new < t_peph(end-left),1 ,'last');
    
    x_interp = NaN(size(t_new));
    for ii = st:en    
    
        % find closest n_lagrange indices
        [~, sortedIndices] = sort(abs(t_peph-t_new(ii)));
        idx = sort(sortedIndices(1:n_lagrange_polynomial));
    
        x = t_peph(idx);
        y = peph(idx,1);
        x_interp(ii) = lagrange_interp(x, y, t_new(ii));
    end
    

    % subplot(3,1,1);
    plot(t_new, x_interp, '.-')
    plot(t_peph, peph(:,1), 'x')
    xlim([t_new(1) t_new(end)])
end

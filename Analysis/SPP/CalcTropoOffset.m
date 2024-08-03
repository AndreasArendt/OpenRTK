function [tropo_offset] = CalcTropoOffset(x_rx, y_rx, z_rx, elevation, varargin)
%CALCTROPOOFFSET Summary of this function goes here
%   Detailed explanation goes here

    ip = inputParser();
    ip.addParameter('Model', 'Saastamoinen');

    ip.parse(varargin{:});

    [lat__rad, ~, alt__m] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);    
    if alt__m > 0 && alt__m < 2000
        
        if strcmp(ip.Results.Model, 'Saastamoinen')
            tropo_offset     =  Troposphere.Saastamoinen_TropoModel(lat__rad, alt__m, elevation); 
        elseif strcmp(ip.Results.Model, 'Collins')
            tropo_offset = Troposphere.Collins_TropoModel(alt__m, elevation);
        else
            fprintf(2, 'Not tropo Model selected!\n');
            tropo_offset = zeros(size(elevation));
        end
    else
        tropo_offset = zeros(size(elevation));
    end    
end


function [tropo_offset] = CalcTropoOffset(x_rx, y_rx, z_rx, elevation)
%CALCTROPOOFFSET Summary of this function goes here
%   Detailed explanation goes here

    [lat__rad, ~, alt__m] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);    
    if alt__m > 0 && alt__m < 2000
        
        tropo_offset     =  Troposphere.Saastamoinen_TropoModel(lat__rad, alt__m, elevation); 
        tropo_offset_old = Troposphere.Collins_TropoModel(alt__m, elevation);

        % tropo_offset = tropo_offset_old;
    else
        tropo_offset = zeros(size(elevation));
    end    
end


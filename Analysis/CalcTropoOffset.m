function [tropo_offset] = CalcTropoOffset(x_rx, y_rx, z_rx, dx, dy, dz)
%CALCTROPOOFFSET Summary of this function goes here
%   Detailed explanation goes here

    [lat__rad, lon__rad, alt__m] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);                
    [E,N,U] = Transformation.ecef2enu(lat__rad, lon__rad, dx, dy, dz);
    
    elevation = atan2(U, sqrt(E.^2 + N.^2));

    tropo_offset_old = TropoModel(alt__m, elevation);

    if alt__m > 0 && alt__m < 2000
        % tropo_offset =  Saastamoinen_TropoModel(lat__rad, alt__m, elevation);
        tropo_offset = tropo_offset_old;
    else
        tropo_offset = zeros(size(elevation));
    end    
end


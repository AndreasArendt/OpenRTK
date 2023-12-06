function [tropo_offset] = CalcTropoOffset(x_rx, y_rx, z_rx, dx, dy, dz)
%CALCTROPOOFFSET Summary of this function goes here
%   Detailed explanation goes here

    [lat__rad, lon__rad, alt__m] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);                
    [E,N,U] = Transformation.ecef2enu(lat__rad, lon__rad, dx, dy, dz);
    
    elevation = atan2(U, sqrt(E.^2 + N.^2));

    tropo_offset = TropoModel(alt__m, elevation);

end


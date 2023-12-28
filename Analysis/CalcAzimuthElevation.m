function [azimuth, elevation] = CalcAzimuthElevation(x_E__m, y_E__m, z_E__m, d_x, d_y, d_z)
%CALCAZIMUTHELEVATION Summary of this function goes here
%   Detailed explanation goes here
    [lat__rad, lon__rad, ~] = Transformation.ecef2wgs84(x_E__m, y_E__m, z_E__m);                
    [E,N,U] = Transformation.ecef2enu(lat__rad, lon__rad, d_x, d_y, d_z);
    elevation = atan2(U, sqrt(E.^2 + N.^2));    
    azimuth = atan2(E,N);    
end


function azimuth__rad = calcAzimuth(x_E__m, y_E__m, z_E__m, d_x, d_y, d_z)
%CALCAZIMUTHELEVATION Summary of this function goes here
%   Detailed explanation goes here
    [lat__rad, lon__rad, ~] = Transformation.ecef2wgs84(x_E__m, y_E__m, z_E__m);                
    [E,N,~] = Transformation.ecef2enu(lat__rad, lon__rad, d_x, d_y, d_z);    
    azimuth__rad = atan2(E,N);    
end


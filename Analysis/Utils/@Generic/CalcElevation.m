function elevation__rad = CalcElevation(x_rx, y_rx, z_rx, dx, dy, dz)

    [lat__rad, lon__rad, ~] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);                
    [E,N,U] = Transformation.ecef2enu(lat__rad, lon__rad, dx, dy, dz);
    
    elevation__rad = atan2(U, sqrt(E.^2 + N.^2));    
end
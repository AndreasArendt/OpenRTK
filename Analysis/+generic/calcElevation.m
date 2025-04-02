function elevation__rad = calcElevation(x_rx, y_rx, z_rx, x_sv, y_sv, z_sv)
    e = Vector.NormalizedDistanceVector(x_sv, y_sv, z_sv, x_rx, y_rx, z_rx);

    [lat__rad, lon__rad, ~] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);

    e_zenith = [cos(lat__rad) * cos(lon__rad), ...
                cos(lat__rad) * sin(lon__rad), ...
                sin(lat__rad)];

    e_zenith_vec = repmat(e_zenith, numel(x_sv), 1);   
    elevation__rad = asin(dot(e, e_zenith_vec, 2));  
end
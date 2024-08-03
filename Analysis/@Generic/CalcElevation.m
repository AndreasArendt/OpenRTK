function elevation__rad = CalcElevation(x_rx, y_rx, z_rx, x_sv, y_sv, z_sv)
    e = Vector.NormalizedDistanceVector(x_sv, y_sv, z_sv, x_rx, y_rx, z_rx);

    [lat__rad, lon__rad, ~] = Transformation.ecef2wgs84(x_rx, y_rx, z_rx);

    e_zenith = [cos(lat__rad) * cos(lon__rad); ...
                cos(lat__rad) * sin(lon__rad); ...
                sin(lat__rad)];

    elevation__rad = asin(e * e_zenith);
end
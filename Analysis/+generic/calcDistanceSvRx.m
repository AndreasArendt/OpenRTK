function dist__m = calcDistanceSvRx(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E)
% Calc Distance 
    dist = Vector.EuclidianDistance_3D(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E);
    
    % Sagnac Correction
    sag = Transformation.CalcSagnac( [eph_x_E__m, eph_y_E__m, eph_z_E__m], [x_est_E, y_est_E, z_est_E]);
    
    % correct geographic distance of earth rotation
    dist__m = dist + sag;
end
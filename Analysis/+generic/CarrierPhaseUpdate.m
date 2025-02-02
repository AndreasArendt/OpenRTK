function [A, y] = CarrierPhaseUpdate(x_est_E, y_est_E, z_est_E, rx_clock_offset_est__m, zwd_est__m, ambig_est, Carrierphases, eph_x_E__m, eph_y_E__m, eph_z_E__m, sv_clock_offset__m, sv_relativistic__m)

    % distance between satellites and estimated position - sagnac effect corrected    
    dist_svrx__m = generic.calcDistanceSvRx(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E);

    % elevation mask
    elevation = Generic.CalcElevation(x_est_E, y_est_E, z_est_E, eph_x_E__m, eph_y_E__m, eph_z_E__m);
    
    % Troposphere Model
    [lat__rad, ~, alt__m] = Transformation.ecef2wgs84(x_est_E, y_est_E, z_est_E);    
    tropo_offset = Troposphere.Saastamoinen_TropoModel(lat__rad, alt__m, elevation, zwd_est__m);  

    % Iono-Free LC
    phi_iono_free = Generic.CalcIonoFreeLinearCombination([Carrierphases.Band_1]', [Carrierphases.Band_2]', gnss.F_L1_GPS__Hz, gnss.F_L2_GPS__Hz);

    % Wavelength
    lambda = Generic.CalcIonoFreeLinearCombination(const.c__mDs / gnss.F_L1_GPS__Hz, const.c__mDs / gnss.F_L2_GPS__Hz, gnss.F_L1_GPS__Hz, gnss.F_L2_GPS__Hz);

    e = Vector.NormalizedDistanceVector(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E);    
    Mw = Troposphere.MappingFunction.Chao_MW(elevation);
    L = eye(numel(phi_iono_free)) * lambda;
    A = [-e, ones(numel(e(:,1)),1), Mw, L];                
    
    dist_est = phi_iono_free * lambda + sv_clock_offset__m + sv_relativistic__m - rx_clock_offset_est__m - tropo_offset - lambda * ambig_est.';
    y = dist_est - dist_svrx__m;
end
function [A, y] = PseudorangeUpdate(x_est_E, y_est_E, z_est_E, rx_clock_offset_est__m, zwd_est__m, Pseudoranges__m, eph_x_E__m, eph_y_E__m, eph_z_E__m, sv_clock_offset__m, sv_relativistic__m)
%GETPSEUDORANGEDESIGNMATRIX Summary of this function goes here
%   Detailed explanation goes here

    % Calc Distance 
    dist = Vector.EuclidianDistance_3D(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E);
    
    % Sagnac Correction
    sag = Transformation.CalcSagnac( [eph_x_E__m, eph_y_E__m, eph_z_E__m], [x_est_E, y_est_E, z_est_E]);
    
    % correct geographic distance of earth rotation
    geo_dist = dist + sag;
    
    % elevation mask
    elevation = Generic.CalcElevation(x_est_E, y_est_E, z_est_E, eph_x_E__m, eph_y_E__m, eph_z_E__m);
    
    % Troposphere Model
    [lat__rad, ~, alt__m] = Transformation.ecef2wgs84(x_est_E, y_est_E, z_est_E);    
    tropo_offset = Troposphere.Saastamoinen_TropoModel(lat__rad, alt__m, elevation, zwd_est__m);     
    
    % Iono-Free LC
    rho_iono_free = Generic.CalcIonoFreeLinearCombination([Pseudoranges__m.Band_1]', [Pseudoranges__m.Band_2]', gnss.F_L1_GPS__Hz, gnss.F_L2_GPS__Hz);
           
    % Design Matrix
    e = Vector.NormalizedDistanceVector(eph_x_E__m, eph_y_E__m, eph_z_E__m, x_est_E, y_est_E, z_est_E);    
    Mw = Troposphere.MappingFunction.Chao_MW(elevation);
    A = [-e, ones(numel(e(:,1)),1), Mw];                
    
    dist_est = rho_iono_free + sv_clock_offset__m + sv_relativistic__m - rx_clock_offset_est__m - tropo_offset;
    y = dist_est - geo_dist;
end


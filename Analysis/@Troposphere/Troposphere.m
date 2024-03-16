classdef Troposphere < handle
    %TROPOSPHERE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (Constant = true)
        
    end
    
    methods (Access = public, Static = true)        
        [p__Pa, T__K, rho__kgDm3] = StandardAtmosphere(alt__m)

        % Troposphere MOdels
        dt = Saastamoinen_TropoModel(lat__rad, alt__m, elevation)
        dt = Collins_TropoModel(h_sealevel, sat_elev)
    end
end


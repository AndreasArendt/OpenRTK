function dt = Saastamoinen_TropoModel(lat__rad, alt__m, elevation, zwd)
   
    if alt__m < 0
        alt__m = 0;    
    end
        
    [p__Pa, ~, ~] = ISA.StandardAtmosphere(alt__m);
    p__hPa = p__Pa / 100;
    
    % saastamoninen model
    % Zenith Hyrdostatic delay (ZHD) (dry)
    cSaastamoinen__mDhPa = 0.0022768;
    zhd = (cSaastamoinen__mDhPa * p__hPa)./(1-0.00266*cos(2.0*lat__rad)-0.28e-6*alt__m);

    % Mapping functions by CHAO [Handbook 6.61 + 6.62]    
    dt = Troposphere.MappingFunction.Chao_MH(elevation) .* zhd +  Troposphere.MappingFunction.Chao_MW(elevation) * zwd;
end
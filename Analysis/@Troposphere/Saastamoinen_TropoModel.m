function dt = Saastamoinen_TropoModel(lat__rad, alt__m, elevation)
   
    if alt__m < 0
        alt__m = 0;    
    end
        
    [p__Pa, T__K, ~] = Troposphere.StandardAtmosphere(alt__m);

    p__hPa = p__Pa / 100;

    % water vapour pressure (Magnus)
    e_magnus = 0.61094*exp((17.625*(T__K-273.15))/(T__K+243.04-273.15))/1e3;
    
    % saastamoninen model
    trph=0.0022768.*p__hPa./(1-0.00266*cos(2.0*lat__rad)-0.28e-6*alt__m)./sin(elevation);
    trpw=0.0022768*(1255 + 0.05*T__K)*e_magnus./T__K;
    dt = trph+trpw;
end
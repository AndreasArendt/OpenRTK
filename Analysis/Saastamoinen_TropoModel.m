function dt = Saastamoinen_TropoModel(lat__rad, alt__rad, elevation)

    humi = 0.7;

    temp0=15.0; % temparature at sea level
     
    % standard atmosphere
    if alt__rad < 0.0
        hgt = 0;
    else
        hgt = alt__rad;
    end

    pres=1013.25 * (1.0-2.2557E-5*hgt)^5.2568;
    temp=temp0-6.5E-3*hgt+273.16;
    e=6.108*humi*exp((17.15*temp-4684.0)/(temp-38.45));
   
    % saastamoninen model
    z=pi/2.0-elevation;
    trph=0.0022768 * pres ./ (1.0-0.00266 * cos(2.0*lat__rad) -0.00028*hgt / 1E3) ./ cos(z);
    trpw=0.002277*(1255.0./temp+0.05)*e./cos(z);
    dt = trph+trpw;
end
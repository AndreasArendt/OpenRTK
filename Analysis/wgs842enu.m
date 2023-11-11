function [de,dn,du] = wgs842enu(lat, lon, vector)
    dx = vector(:,1);
    dy = vector(:,2);
    dz = vector(:,3);
    sinl = sin(lon);
    sinb = sin(lat);
    cosl = cos(lon);
    cosb = cos(lat);
    de = -sinl.*dx + cosl.*dy;
    dn = -cosl.*sinb.*dx - sinl.*sinb.*dy + cosb.*dz;
    du = cosl.*cosb.*dx + sinl.*cosb.*dy + sinb.*dz;    
end


function [lat__rad, lon__rad, alt__m] = ecef2wgs84(x__m, y__m, z__m) 
%input in m, output in rad and meters

    % WGS84 ellipsoid constants:
    a = 6378137;
    es = (8.1819190842622e-2)^2;
    
    % calculations:
    b   = sqrt(a^2*(1-es));
    ep  = (a^2-b^2)/b^2;
    
    p   = sqrt(x__m.^2+y__m.^2);
    th  = atan2(a*z__m,b*p);
    lon__rad = atan2(y__m,x__m);
    lat__rad = atan2((z__m+ep.*b.*sin(th).^3),(p-es.*a.*cos(th).^3));
    N   = a./sqrt(1-es.*sin(lat__rad).^2);
    alt__m = p./cos(lat__rad)-N;
    
    % return lon in range [0,2*pi)
    %lon = mod(lon,2*pi); %atan2 should handle this fine
    
    % correct for numerical instability in altitude near exact poles:
    % (after this correction, error is about 2 millimeters, which is about
    % the same as the numerical precision of the overall function)    
    k=abs(x__m)<1 & abs(y__m)<1;
    alt__m(k) = abs(z__m(k))-b;
end
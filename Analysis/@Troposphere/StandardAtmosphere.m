function [p__Pa, T__K, rho__kgDm3] = StandardAtmosphere(alt__m)
% Calculate Standard Atmosphere Parameter
% p__Pa Pressure in Pascal
% T__K Temperature in Kelvin
% rho__kgDm3 Air density in kg/m^3
% data validated with: https://www.dwd.de/DE/service/lexikon/begriffe/S/Standardatmosphaere_pdf.pdf?__blob=publicationFile&v=3

    p0__Pa = 101325;         % Pressure at MSL
    g__mDs2 = 9.80665;       % Gravity
    T0__K = 15 + 273.15;     % Temperature at MSL
    M_kgDmol = 0.02896968;   % Molar Mass   
    R0_JDmolK = 8.314462618; % Universal Gas constant    
    kappa = 1.235;           % heat capacity ratio

    % This model is valid up to 11km
    if alt__m < 0
        alt__m = 0;
    elseif alt__m > 11000
        alt__m = 11000;
    end
        
    T__K = T0__K - 6.5 * alt__m/1000;        
   
    p__Pa = p0__Pa * (1 - ((kappa - 1) / kappa) * ( ...
        ((M_kgDmol*g__mDs2*alt__m) / (R0_JDmolK * T0__K)) ...
        ))^(kappa/(kappa-1));

    rho__kgDm3 = (p0__Pa * M_kgDmol) / (R0_JDmolK * T__K);        
end
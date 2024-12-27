function [p__Pa, T__K, rho__kgDm3] = StandardAtmosphere(alt__m)
% Calculate Standard Atmosphere Parameter
% p__Pa Pressure in Pascal
% T__K Temperature in Kelvin
% rho__kgDm3 Air density in kg/m^3
% Data validated with: https://www.dwd.de/DE/service/lexikon/begriffe/S/Standardatmosphaere_pdf.pdf?__blob=publicationFile&v=3

    % Constants
    p0__Pa = 101325;         % Pressure at MSL in Pascal
    g__mDs2 = 9.80665;       % Gravity in m/s^2
    T0__K = 15 + 273.15;     % Temperature at MSL in Kelvin
    M__kgDmol = 0.02896968;   % Molar Mass of air in kg/mol   
    R0__JDmolK = 8.314462618; % Universal Gas constant in J/(mol*K)
    L__KDkm = 6.5;            % Temperature lapse rate in K/km

    % Limit altitude to 0 <= alt__m <= 11000
    alt__m = max(-1000, min(alt__m, 11000));

    % Calculate temperature
    T__K = T0__K - L__KDkm * alt__m / 1000;

    % Calculate pressure
    p__Pa = p0__Pa * (T__K / T0__K).^(g__mDs2 * M__kgDmol / (R0__JDmolK * (L__KDkm / 1000)));

    % Calculate air density
    rho__kgDm3 = (p__Pa * M__kgDmol) ./ (R0__JDmolK * T__K);
end

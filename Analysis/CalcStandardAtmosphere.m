function Pressure__Pa = CalcStandardAtmosphere(alt__m)

p0__Pa = 101325;
g__mDs2 = 9.81;
T0__K = 288.16;
M_kgDmol = 0.02896968;
C_P__JDkgK = 1004.68506;
R0_JDmolK = 8.314462618;

if alt__m < 0
    alt__m = 0;
elseif alt__m > 29500
    alt__m = 29500;
end

Pressure__Pa = p0__Pa * (1 - (g__mDs2 * alt__m) / (C_P__JDkgK * T0__K))^((C_P__JDkgK * M_kgDmol) / R0_JDmolK);

end
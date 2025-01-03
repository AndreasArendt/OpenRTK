classdef Generic < handle
    %GENERIC Summary of this class goes here
    %   Detailed explanation goes here
    
    methods (Static = true)
        elevation__rad = CalcElevation(x_rx, y_rx, z_rx, dx, dy, dz);
        azimuth__rad   = CalcAzimuth(x_rx, y_rx, z_rx, dx, dy, dz);

        r_ionofree = CalcIonoFreeLinearCombination(x_1, x_2, F1, F2);

        DictPeph = GetPreciseEphemeris(PreciseEphemeris, Satellite, time__s, options);
        DictPclk = GetPreciseClock(PreciseCloick, Satellite, time__s);
    end
end


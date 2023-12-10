function [delta_carrierphase] = CalcDeltaCarrierphase(x_rx, y_rx, z_rx, phase_1, phase_2, F1, F2, dt_sv, cdt_rx, dx, dy, dz)
%CALCDELTACARRIERPHASE Summary of this function goes here
%   Detailed explanation goes here

    cdt_sv  = Transformation.SpeedOfLight__mDs * dt_sv;  

    tropo_offset = CalcTropoOffset(x_rx, y_rx, z_rx, dx, dy, dz);

    dist_F1 = phase_1 .* Transformation.SpeedOfLight__mDs ./ F1;    
    dist_F2 = phase_2 .* Transformation.SpeedOfLight__mDs ./ F2;    

    [r_ionofree] = CalcIonoFreeLinearCombination(dist_F1, dist_F2, F1, F2);    
    delta_carrierphase = r_ionofree - cdt_rx + cdt_sv - tropo_offset; % phase windup missing here!
end


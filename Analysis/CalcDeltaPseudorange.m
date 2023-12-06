function [delta_pseudorange] = CalcDeltaPseudorange(x_rx, y_rx, z_rx, Code_1, Code_2, F1, F2, dt_sv, dt_rx, dt_rel, dx, dy, dz)
%CALCDELTAPSEUDORANGE Summary of this function goes here
%   Detailed explanation goes here
    cdt_sv  = Transformation.SpeedOfLight__mDs * dt_sv;
    cdt_rx = dt_rx;
    cdt_rel = Transformation.SpeedOfLight__mDs * dt_rel;
            
    tropo_offset = CalcTropoOffset(x_rx, y_rx, z_rx, dx, dy, dz);
    r_ionofree = CalcIonoFreePseudorange(Code_1, Code_2, F1, F2);
    
    delta_pseudorange = r_ionofree - cdt_rx + cdt_sv - tropo_offset - cdt_rel;          
end


function [r_ionofree] = CalcIonoFreePseudorange(Code_1, Code_2, F1, F2)
%CALCIONOFREEPSEUDORANGE Summary of this function goes here
%   Detailed explanation goes here

    r_ionofree = ((F1^2 * Code_1) - (F2^2 * Code_2)) ./ (F1^2 - F2^2);
end


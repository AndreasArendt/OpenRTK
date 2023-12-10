function [r_ionofree] = CalcIonoFreeLinearCombination(x_1, x_2, F1, F2)
%CalcIonoFreeLinearCombination Summary of this function goes here
%   Detailed explanation goes here

    r_ionofree = ((F1^2 * x_1) - (F2^2 * x_2)) ./ (F1^2 - F2^2);
end


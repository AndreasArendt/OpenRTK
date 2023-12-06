function [H ,z, R] = TestCorrection1(x)
    H = acos(x);%-sin(x);
    z = x;
    R = 1;
end
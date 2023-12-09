function [dist] = CalcDeltaCarrierphase(phase, Freq)
%CALCDELTACARRIERPHASE Summary of this function goes here
%   Detailed explanation goes here

    dist = phase .* Transformation.SpeedOfLight__mDs ./ Freq;    
end


function [r,d,e] = CalcDistance(x_sv, y_sv, z_sv, x_rx, y_rx, z_rx)
%CALCPSEUDORANGERESIDUALS Summary of this function goes here
%   Detailed explanation goes here
    dx = (x_sv - x_rx);
    dy = (y_sv - y_rx);
    dz = (z_sv - z_rx);
    r = sqrt( dx.^2 + dy.^2 + dz.^2);

    d = [dx, dy, dz];
    e = d ./ r;    

    % Sagnac Effect
    r = r + Transformation.MeanAngularVelocityOfEarth__radDs .* (x_sv .* y_rx - y_sv .* x_rx) ./ Transformation.SpeedOfLight__mDs;
end


function [doppler_est__Hz] = CalcDeltaDoppler(vel_x_sv, vel_y_sv, vel_z_sv, vel_x_rx, vel_y_rx, vel_z_rx, dt_dot_sv, cdt_dot_rx, e,F)
%CALCDELTADOPPLER Summary of this function goes here
%   Detailed explanation goes here
    delta_vel = [ vel_x_sv, vel_y_sv, vel_z_sv] - ...
                [ vel_x_rx, vel_y_rx, vel_z_rx];

    delta_clock_drift = cdt_dot_rx - Transformation.SpeedOfLight__mDs .* dt_dot_sv;

    doppler_est__Hz = (-1/(Transformation.SpeedOfLight__mDs / F)) .* (dot(e, delta_vel,2) - delta_clock_drift);

end


function dt = Collins_TropoModel(h_sealevel, sat_elev)
    % [COLLINS, 1999], valid for elevations >5deg!
    % ref: https://gssc.esa.int/navipedia/index.php/Tropospheric_Delay
    d_dry = 2.3 .* exp(-0.116E-03 .* h_sealevel);

    if isinf(d_dry)
        d_dry = 1e6;
    end

    d_wet = 0.1;
    m_elev = 1.001./sqrt(0.002001 + sin(sat_elev).^2);
    dt = (d_dry + d_wet) .* m_elev;
end
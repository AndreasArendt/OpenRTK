function dt = TropoModel(h_sealevel, sat_elev)
    d_dry = 2.3 .* exp(-0.116E-03 .* h_sealevel);

    if isinf(d_dry)
        d_dry = 1e6;
    end

    d_wet = 0.1;
    m_elev = 1.001./sqrt(0.002001 + sin(sat_elev).^2);
    dt = (d_dry + d_wet) .* m_elev;
end
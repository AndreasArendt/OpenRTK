function Mw = Chao_MW(elevation)
    Mw = 1./(sin(elevation) + (0.00035./(tan(elevation)+0.0017)));
end
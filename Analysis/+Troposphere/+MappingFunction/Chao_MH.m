function Mh = Chao_MH(elevation)
    Mh = 1./(sin(elevation) + (0.00143./(tan(elevation)+0.0445)));
end
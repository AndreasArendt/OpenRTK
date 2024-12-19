clear; fclose all;
SatelliteData = Load();

% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
% station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

station_pos__m = [ -4716238.5966  2613288.5223 -3396264.3755];

F_E1_Galileo__Hz  = 1575.420e6;
F_E5a_Galileo__Hz = 1176.450e6;
F_L2_GPS__Hz      = 1227.600e6;

function SatelliteData = Load()
    try
        fid = fopen('data/satdata.json');
        j_str = fscanf(fid, '%s');
    catch
        fclose(fid);
    end
    fclose(fid);
    
    s = jsondecode(j_str);
    SatelliteData = s.SatelliteData;              
end
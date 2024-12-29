clear; fclose all;

SatelliteData = loadSatdata();
PreciseEphemerisData = loadPephdata();
PreciseClockData = loadPclkdata();

% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
% station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

% station_pos__m = [ -4716238.5966  2613288.5223 -3396264.3755];

F_E1_Galileo__Hz  = 1575.420e6;
F_E5a_Galileo__Hz = 1176.450e6;
F_L2_GPS__Hz      = 1227.600e6;

function SatelliteData = loadSatdata()    
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', 'satdata.json');        
    s = json.read(fpath);
    SatelliteData = s.SatelliteData;              
end

function PreciseEphemerisData = loadPephdata()  
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', 'precise_satdata.json');        
    peph = json.read(fpath);
    PreciseEphemerisData = peph.PreciseEphemerisData;
end

function PreciseClockData = loadPclkdata()
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', 'precise_clkdata.json');        
    pclk = json.read(fpath);
    PreciseClockData = pclk.PreciseClockData;
end
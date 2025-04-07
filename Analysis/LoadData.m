clear; fclose all;

SatelliteData = loadSatdata();
PreciseEphemerisData = loadPephdata();
PreciseClockData = loadPclkdata();

meta = generic.getMeta(SatelliteData);

function SatelliteData = loadSatdata()    
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', '2025_009', '204', 'satdata.json');        
    fpath = path.canonicalPath(fpath);
    SatelliteData = [];

    if exist(fpath, "file") == 2
        s = json.read(fpath);
        SatelliteData = s.SatelliteData;              
    end
end

function PreciseEphemerisData = loadPephdata()  
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', 'precise_satdata.json');            
    fpath = path.canonicalPath(fpath);

    PreciseEphemerisData = [];

    if exist(fpath, "file") == 2
        peph = json.read(fpath);
        PreciseEphemerisData = peph.PreciseEphemerisData;
    end
end

function PreciseClockData = loadPclkdata()
    fpath = fullfile(mfilename('fullpath'), '..', '..', 'data', 'precise_clkdata.json');        
    fpath = path.canonicalPath(fpath);

    PreciseClockData = [];

    if exist(fpath, "file") == 2
        pclk = json.read(fpath);
        PreciseClockData = pclk.PreciseClockData;
    end
end
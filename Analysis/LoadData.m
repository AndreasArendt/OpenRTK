function [NavData, SatelliteData, PreciseEphemerisData, PreciseClockData, meta] = LoadData(folderpath)
    % folderpath = 'D:\spoof_dataset\rnd\00bbaf1439124529afc84508933e3bfd';
    
    SatelliteData = loadSatdata(folderpath);
    PreciseEphemerisData = loadPephdata(folderpath);
    PreciseClockData = loadPclkdata(folderpath);
    NavData = loadNavdata(folderpath);
    
    meta = generic.getMeta(SatelliteData);
end

function NavData = loadNavdata(folderpath)    
    fpath = fullfile(folderpath, 'navdata.json');
    fpath = path.canonicalPath(fpath);
    NavData = [];

    if exist(fpath, "file") == 2
        s = json.read(fpath);
        NavData = s.SatelliteData;              
    end
end

function SatelliteData = loadSatdata(folderpath)    
    fpath = fullfile(folderpath, 'satdata.json');
    fpath = path.canonicalPath(fpath);
    SatelliteData = [];

    if exist(fpath, "file") == 2
        s = json.read(fpath);
        SatelliteData = s.SatelliteData;              
    end
end

function PreciseEphemerisData = loadPephdata(folderpath)      
    fpath = fullfile(folderpath, 'precise_satdata.json');
    fpath = path.canonicalPath(fpath);

    PreciseEphemerisData = [];

    if exist(fpath, "file") == 2
        peph = json.read(fpath);
        PreciseEphemerisData = peph.PreciseEphemerisData;
    end
end

function PreciseClockData = loadPclkdata(folderpath)        
    fpath = fullfile(folderpath, 'precise_clkdata.json');
    fpath = path.canonicalPath(fpath);

    PreciseClockData = [];

    if exist(fpath, "file") == 2
        pclk = json.read(fpath);
        PreciseClockData = pclk.PreciseClockData;
    end
end
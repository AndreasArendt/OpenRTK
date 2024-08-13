function [px] = epoch2posix(day, month, year, hour, minute, second)
    dt = datetime(year, month, day, hour, minute, second);        
    px = posixtime(dt);
end
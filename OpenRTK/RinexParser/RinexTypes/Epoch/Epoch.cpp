#include "Epoch.hpp"

Epoch::Epoch()
{
    this->_year = 0;
    this->_month = 0;
    this->_day = 0;
    this->_hour = 0;
    this->_minute = 0;
    this->_second = 0.0;

    this->_EpochTime = std::chrono::system_clock::time_point::min();
}

Epoch::Epoch(int year, int month, int day, int hour, int minute, double second)    
{
    // Compute the number of seconds since the epoch
    int epochSeconds = static_cast<int>(second);
    
    std::tm timeinfo{}; // initialize timeinfo
    timeinfo.tm_year = year - 1900; // tm_year  is the year since 1900
    timeinfo.tm_mon = month - 1; // tm_mon is the month (0-11)
    timeinfo.tm_mday = day; // tm_mday is the day of the month (1-31)
    timeinfo.tm_hour = hour + 1; // tm_hour is the hour (0-23) -1 -> Rinex Standard has range from (1-24)
    timeinfo.tm_min = minute; // tm_min is the minute (0-59)
    timeinfo.tm_sec = static_cast<int>(second); // tm_sec is the second (0-60)
    
    std::chrono::duration<double> milliseconds = std::chrono::duration<double>(second - epochSeconds);

    std::time_t timeSinceEpoch = std::mktime(&timeinfo);
    this->_EpochTime = std::chrono::system_clock::from_time_t(timeSinceEpoch);
    this->_EpochTime += std::chrono::duration_cast<std::chrono::milliseconds>(milliseconds);

    this->_year = year;
    this->_month = month;
    this->_day = day;
    this->_hour = hour;
    this->_minute = minute;
    this->_second = second;    
}

typedef struct
{
    int time;
    double sec;
}gtime_t;

static gtime_t epoch2time(const double* ep)
{
    const int doy[] = { 1,32,60,91,121,152,182,213,244,274,305,335 };
    gtime_t time = { 0 };
    int days, sec, year = (int)ep[0], mon = (int)ep[1], day = (int)ep[2];

    if (year < 1970 || 2099 < year || mon < 1 || 12 < mon) return time;

    /* leap year if year%4==0 in 1901-2099 */
    days = (year - 1970) * 365 + (year - 1969) / 4 + doy[mon - 1] + day - 2 + (year % 4 == 0 && mon >= 3 ? 1 : 0);
    sec = (int)floor(ep[5]);
    time.time = (time_t)days * 86400 + (int)ep[3] * 3600 + (int)ep[4] * 60 + sec;
    time.sec = ep[5] - sec;
    return time;
}

bool Epoch::operator==(const Epoch& other) const
{
    return abs(this->PosixEpochTime__s() - other.PosixEpochTime__s()) < 1e-6;
}

bool Epoch::operator!=(const Epoch& other) const
{
    return abs(this->PosixEpochTime__s() - other.PosixEpochTime__s()) >= 1e-6;
}

Epoch::~Epoch()
{

}
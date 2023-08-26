#include "Epoch.hpp"

#include "../../NavData/rtktypes.h"

Epoch::Epoch()
{
    this->_year = 0;
    this->_month = 0;
    this->_day = 0;
    this->_hour = 0;
    this->_minute = 0;
    this->_second = 0.0;
}

Epoch::Epoch(int year, int month, int day, int hour, int minute, double second)    
{
    // Compute the number of seconds since the epoch
    int epochSeconds = static_cast<int>(second);
    
    std::tm timeinfo{}; // initialize timeinfo
    timeinfo.tm_year = year - 1900; // tm_year is the year since 1900
    timeinfo.tm_mon = month - 1; // tm_mon is the month (0-11)
    timeinfo.tm_mday = day; // tm_mday is the day of the month (1-31)
    timeinfo.tm_hour = hour + 1; // tm_hour is the hour (0-23) -1 -> Rinex Standard has range from (1-24)
    timeinfo.tm_min = minute; // tm_min is the minute (0-59)
    timeinfo.tm_sec = epochSeconds; // tm_sec is the second (0-60)
    
    std::time_t timeSinceEpoch = std::mktime(&timeinfo);

    // Create a time_point object from the time_t value
    this->_EpochTime = std::chrono::system_clock::from_time_t(timeSinceEpoch);

    this->_year = year;
    this->_month = month;
    this->_day = day;
    this->_hour = hour;
    this->_minute = minute;
    this->_second = second;
}

/**

    @brief Converts the epoch time to UTC in seconds.
    @return The epoch time converted to UTC in seconds.
    */
double Epoch::ConvertEpochTimeToUTC()
{
    auto duration_since_epoch = this->_EpochTime.time_since_epoch();
    return std::chrono::duration<double>(duration_since_epoch).count();
}

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

// TOC
//double Epoch::Toc()
//{
//    int dayOfYear = Time::get_yday(this->_month, this->_day, this->_year);
//
//    double xy = static_cast<double>(this->_year);
//    int id_GPS = static_cast<int>(365.25 * (xy - 1.0)) + dayOfYear - 722835;
//
//    // Day of week:
//    int idw = id_GPS % 7;
//
//    // Number of GPS week:
//    int nw = (id_GPS - idw) / 7;
//
//    // seconds in the week:
//    double t = static_cast<double>(idw) * 86400.0 + (int)this->_hour * 3600 + (int)this->_minute * 60 + this->_second;
//    return t;
//}

double Epoch::Toc()
{
    double ep[6] =
    {
        (double)this->_year, (double)this->_month, (double)this->_day, (double)this->_hour, (double)this->_minute, (double)this->_second
    };

    auto t = epoch2time(ep);
    
    return (double)t.time + t.sec;
}

bool Epoch::operator==(const Epoch& other) const
{
    return _EpochTime == other.EpochTime();
}

bool Epoch::operator!=(const Epoch& other) const
{
    return _EpochTime != other.EpochTime();
}

Epoch::~Epoch()
{

}
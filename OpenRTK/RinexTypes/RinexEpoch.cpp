#include "RinexEpoch.hpp"

RinexEpoch::RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs)
    : _EpochTime{} 
    , _EpochFlag{ epochFlag }
    , _NumberSVs{ numberSVs }
    , _IsSpecialEvent{ epochFlag >= 2 }
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
}

void RinexEpoch::AddCodeObservation(CodeObservation& observation)
{
    _CodeObservations.emplace_back(observation);
}

void RinexEpoch::AddPhaseObservation(PhaseObservation& observation)
{
    _PhaseObservations.emplace_back(observation);
}

void RinexEpoch::AddDopplerObservation(DopplerObservation& observation)
{
    _DopplerObservations.emplace_back(observation);
}

/**

    @brief Converts the epoch time to UTC in seconds.
    @return The epoch time converted to UTC in seconds.
    */
double RinexEpoch::ConvertEpochTimeToUTC()
{
    auto duration_since_epoch = this->_EpochTime.time_since_epoch();
    return std::chrono::duration<double>(duration_since_epoch).count();
}

RinexEpoch::~RinexEpoch()
{

}
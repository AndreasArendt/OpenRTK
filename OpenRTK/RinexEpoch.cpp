#include "RinexEpoch.hpp"
#include "RinexReader/RinexObservation.hpp"
#include <vector>

std::vector<RinexObservation> _RinexObservations;

RinexEpoch::RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs)
{
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->minute = minute;
    this->second = second;
    this->epochFlag = epochFlag;
    this->numberSVs = numberSVs;

    this->isSpecialEvent = epochFlag < 2; // Special Event are >=2; 0 | 1 are invalid/valid)
}

void RinexEpoch::AddObservation(RinexObservation& observation)
{
    _RinexObservations.emplace_back(observation);
}

RinexEpoch::~RinexEpoch()
{

}
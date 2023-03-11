#include "RinexEpoch.hpp"

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

RinexEpoch::~RinexEpoch()
{

}
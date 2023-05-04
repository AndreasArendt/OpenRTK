#include "ObsData.hpp"

void ObsData::AddCodeObservation(CodeObservation& observation)
{
    this->_CodeObservations.push_back(observation);
}

void ObsData::AddPhaseObservation(PhaseObservation& observation)
{
    this->_PhaseObservations.push_back(observation);
}

void ObsData::AddDopplerObservation(DopplerObservation& observation)
{
    this->_DopplerObservations.push_back(observation);
}

void ObsData::AddSnrObservation(SignalStrengthObservation& observation)
{
    this->_SnrObservations.push_back(observation);
}

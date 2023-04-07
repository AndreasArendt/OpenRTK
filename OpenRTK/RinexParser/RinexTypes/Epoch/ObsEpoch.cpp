#include "ObsEpoch.hpp"

void ObsEpoch::AddCodeObservation(CodeObservation& observation)
{
    this->_CodeObservations.push_back(observation);    
}

void ObsEpoch::AddPhaseObservation(PhaseObservation& observation)
{
    this->_PhaseObservations.push_back(observation);
}

void ObsEpoch::AddDopplerObservation(DopplerObservation& observation)
{
    this->_DopplerObservations.push_back(observation);
}

void ObsEpoch::AddSnrObservation(SignalStrengthObservation& observation)
{
    this->_SnrObservations.push_back(observation);
}

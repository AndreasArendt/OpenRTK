#include "ObsEpoch.hpp"

void ObsEpoch::AddCodeObservation(CodeObservation& observation)
{
    this->_CodeObservations.emplace_back(observation);
}

void ObsEpoch::AddPhaseObservation(PhaseObservation& observation)
{
    this->_PhaseObservations.emplace_back(observation);
}

void ObsEpoch::AddDopplerObservation(DopplerObservation& observation)
{
    this->_DopplerObservations.emplace_back(observation);
}

void ObsEpoch::AddSnrObservation(SignalStrengthObservation& observation)
{
    this->_SnrObservations.emplace_back(observation);
}

#include "ObsData.hpp"

void ObsData::AddCodeObservation(ObservationBand band, double Pseudorange__m)
{
    this->_CodeObservations.emplace(band, Pseudorange__m);
}

void ObsData::AddPhaseObservation(ObservationBand band, double Carrierphase__Cycles)
{
    this->_PhaseObservations.emplace(band, Carrierphase__Cycles);
}

void ObsData::AddDopplerObservation(ObservationBand band, double Doppler__Hz)
{
    this->_DopplerObservations.emplace(band, Doppler__Hz);
}

void ObsData::AddSnrObservation(ObservationBand band, double SNR)
{
    this->_SnrObservations.emplace(band, SNR);
}

#pragma once

#include "RinexObservation.hpp"

class SignalStrengthObservation : public RinexObservation
{
private:
	double _SNR;

public:
	SignalStrengthObservation(const SignalStrengthObservation& other) : RinexObservation(), _SNR(other._SNR) { }
	SignalStrengthObservation(ObservationBand obsBand, std::unique_ptr<Satellite> satellite, double snr) : RinexObservation(obsBand, std::move(satellite)), _SNR(snr) { }	
	~SignalStrengthObservation() { }
};
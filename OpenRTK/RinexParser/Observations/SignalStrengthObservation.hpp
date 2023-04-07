#pragma once

#include "RinexObservation.hpp"

class SignalStrengthObservation : public RinexObservation
{
private:
	double _SNR;

public:		
	SignalStrengthObservation(ObservationBand obsBand, Satellite satellite, double snr) : RinexObservation(obsBand, satellite), _SNR(snr) { }	
	~SignalStrengthObservation() { }
};
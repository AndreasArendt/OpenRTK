#pragma once

#include "RinexObservation.hpp"

class SignalStrengthObservation : public RinexObservation
{
private:
	double _SNR;

public:
	SignalStrengthObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double carrierphase);
	~SignalStrengthObservation();
};
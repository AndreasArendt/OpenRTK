#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class DopplerObservation : public RinexObservation
{
private:
	double _Doppler__Hz;

public:
	DopplerObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double doppler);
	~DopplerObservation();
};


#pragma once
#include "RinexReader/RinexObservation.hpp"
#include "ObservationBand.hpp"

class PhaseObservation : public RinexObservation
{
private:
	double _Carrierphase;

public:
	PhaseObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double carrierphase);
	~PhaseObservation();
};
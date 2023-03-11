#pragma once
#include "../Observations/RinexObservation.hpp"
#include "../ObservationBand.hpp"

class PhaseObservation : public RinexObservation
{
private:
	double _Carrierphase__Cycles;

public:
	PhaseObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double carrierphase);
	~PhaseObservation();
};
#pragma once
#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class PhaseObservation : public RinexObservation
{
private:
	double _Carrierphase__Cycles;

public:		
	PhaseObservation(ObservationBand obsBand, Satellite satellite, double carrierphase) : RinexObservation(obsBand, satellite), _Carrierphase__Cycles(carrierphase) { }	
	~PhaseObservation() { }
};
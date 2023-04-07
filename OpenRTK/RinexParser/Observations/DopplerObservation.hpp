#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class DopplerObservation : public RinexObservation
{
private:
	double _Doppler__Hz;

public:		
	DopplerObservation(ObservationBand obsBand, Satellite satellite, double doppler) : RinexObservation(obsBand, satellite), _Doppler__Hz(doppler) { }	
	~DopplerObservation() { }
};


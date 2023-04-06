#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class DopplerObservation : public RinexObservation
{
private:
	double _Doppler__Hz;

public:
	DopplerObservation(const DopplerObservation& other) : RinexObservation(), _Doppler__Hz(other._Doppler__Hz) { }
	DopplerObservation(ObservationBand obsBand, std::unique_ptr<Satellite> satellite, double doppler) : RinexObservation(obsBand, std::move(satellite)), _Doppler__Hz(doppler) { }	
	~DopplerObservation() { }
};


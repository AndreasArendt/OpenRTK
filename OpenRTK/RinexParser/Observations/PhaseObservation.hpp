#pragma once
#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class PhaseObservation : public RinexObservation
{
private:
	double _Carrierphase__Cycles;

public:
	PhaseObservation(const PhaseObservation& other) : RinexObservation(), _Carrierphase__Cycles(other._Carrierphase__Cycles) { }
	PhaseObservation(ObservationBand obsBand, std::unique_ptr<Satellite> satellite, double carrierphase) : RinexObservation(obsBand, std::move(satellite)), _Carrierphase__Cycles(carrierphase) { }	
	~PhaseObservation() { }
};
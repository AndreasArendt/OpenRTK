#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"
#include "../RinexTypes/Satellite.hpp"

class CodeObservation : public RinexObservation 
{
private:
	double _Pseudorange__m;

public:		
	CodeObservation(const CodeObservation& other) : RinexObservation(), _Pseudorange__m(other._Pseudorange__m) { }
	CodeObservation(ObservationBand obsBand, std::unique_ptr<Satellite> satellite, double pseudorange) : RinexObservation(obsBand, std::move(satellite)), _Pseudorange__m(pseudorange) { }
	~CodeObservation() { }
};


#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"
#include "../RinexTypes/Satellite.hpp"

class CodeObservation : public RinexObservation 
{
private:
	double _Pseudorange__m;

public:				
	CodeObservation(ObservationBand obsBand, Satellite satellite, double pseudorange) : RinexObservation(obsBand, satellite), _Pseudorange__m(pseudorange) { }
	~CodeObservation() { }
};


#pragma once

#include "../Observations/RinexObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

class CodeObservation : public RinexObservation 
{
private:
	double _Pseudorange__m;

public:	
	CodeObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double pseudorange);
	~CodeObservation();
};


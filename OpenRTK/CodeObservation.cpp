#include "CodeObservation.hpp"

CodeObservation::CodeObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double pseudorange) : RinexObservation(obsBand, svSystem, svNumber)
{
	this->_Pseudorange = pseudorange;
}

CodeObservation::~CodeObservation()
{

}

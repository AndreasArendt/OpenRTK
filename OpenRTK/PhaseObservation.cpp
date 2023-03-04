#include "PhaseObservation.hpp"

PhaseObservation::PhaseObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double carrierphase) : RinexObservation(obsBand, svSystem, svNumber)
{
	this->_Carrierphase = carrierphase;
}

PhaseObservation::~PhaseObservation()
{

}
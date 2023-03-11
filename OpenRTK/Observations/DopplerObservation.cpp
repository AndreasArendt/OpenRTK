#include "DopplerObservation.hpp"

DopplerObservation::DopplerObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double doppler) : RinexObservation(obsBand, svSystem, svNumber)
{
	this->_Doppler__Hz = doppler;
}

DopplerObservation::~DopplerObservation()
{

}

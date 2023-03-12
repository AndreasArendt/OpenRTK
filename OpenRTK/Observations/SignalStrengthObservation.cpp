#include "SignalStrengthObservation.hpp"

SignalStrengthObservation::SignalStrengthObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber, double snr) : RinexObservation(obsBand, svSystem, svNumber)
{
	this->_SNR = snr;
}

SignalStrengthObservation::~SignalStrengthObservation()
{

}
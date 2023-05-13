#pragma once

class SignalStrengthObservation
{
private:
	double _SNR;

public:		
	double const& SNR() const { return this->_SNR; }

	SignalStrengthObservation(double snr) : _SNR(snr) { }	
	~SignalStrengthObservation() { }
};
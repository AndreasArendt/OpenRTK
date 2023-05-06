#pragma once

class SignalStrengthObservation
{
private:
	double _SNR;

public:		
	SignalStrengthObservation(double snr) : _SNR(snr) { }	
	~SignalStrengthObservation() { }
};
#pragma once

class DopplerObservation
{
private:
	double _Doppler__Hz;

public:
	double const& Doppler__Hz() const { return this->_Doppler__Hz; }

	DopplerObservation(double doppler) : _Doppler__Hz(doppler) { }
	~DopplerObservation() { }
};


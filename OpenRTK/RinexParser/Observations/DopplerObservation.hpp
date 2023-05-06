#pragma once

class DopplerObservation
{
private:
	double _Doppler__Hz;

public:
	DopplerObservation(double doppler) : _Doppler__Hz(doppler) { }
	~DopplerObservation() { }
};


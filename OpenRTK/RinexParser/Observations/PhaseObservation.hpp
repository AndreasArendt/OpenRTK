#pragma once

class PhaseObservation
{
private:
	double _Carrierphase__Cycles;

public:		
	double const& Carrierphase__Cycles() const { return this->_Carrierphase__Cycles; }

	PhaseObservation(double carrierphase) : _Carrierphase__Cycles(carrierphase) { }	
	~PhaseObservation() { }
};
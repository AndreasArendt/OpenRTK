#pragma once

#include "../Utils/SvSystem.hpp"
#include "../Utils/AbstractSatellite.hpp"

class SP3Satellite : public AbstractSatellite
{
private:
	int _Accuracy;
	

public:	
	// getters
	int const& Accuracy() const { return this->_Accuracy; }

	// ctor & dtor
	SP3Satellite() : _Accuracy(-1) {}
	SP3Satellite(SvSystem svSystem, int svNumber) : AbstractSatellite(svSystem, svNumber), _Accuracy(-1) {}
	SP3Satellite(std::string satStr) : AbstractSatellite(satStr), _Accuracy(-1) {}
	SP3Satellite(const SP3Satellite& other) : AbstractSatellite(other), _Accuracy(-1) {}

	bool operator==(const SP3Satellite& other) const
	{
		return (this->SVSystem() == other.SVSystem()) && (this->SvNumber() == other.SvNumber() && (this->_Accuracy == other.Accuracy()));
	}
};
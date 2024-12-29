#pragma once

#include "../Utils/AbstractSatellite.hpp"
#include "../Utils/Epoch.hpp"

#include <map>

class ClkSatellite : public AbstractSatellite
{
private:
	std::map<Epoch, double> _ClockCorrectionData; // clock correction in seconds

public:	
	// getters
	std::map<Epoch, double> const& ClockCorrectionData() const { return this->_ClockCorrectionData; }

	// functions
	void AddClockCorrectionData(Epoch epoch, double clockCorrection__s)
	{
		this->_ClockCorrectionData[epoch] = clockCorrection__s;
	}

	// ctor & dtor
	ClkSatellite() {}
	ClkSatellite(SvSystem svSystem, int svNumber) : AbstractSatellite(svSystem, svNumber) {}
	ClkSatellite(std::string satStr) : AbstractSatellite(satStr) {}
	ClkSatellite(const ClkSatellite& other) : AbstractSatellite(other) 
	{
		this->_ClockCorrectionData = other._ClockCorrectionData;
	}
};
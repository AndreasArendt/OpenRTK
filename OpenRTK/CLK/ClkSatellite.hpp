#pragma once

#include "../Utils/AbstractSatellite.hpp"
#include "../Utils/Epoch.hpp"

#include <map>

class ClkSatellite : public AbstractSatellite
{
private:
	std::map<Epoch, double> _ClockCorrectionData;

public:	
	// getters
	std::map<Epoch, double> const& ClockCorrectionData() const { return this->_ClockCorrectionData; }

	// functions
	void AddClockCorrectionData(Epoch epoch, double clockCorrection)
	{
		this->_ClockCorrectionData[epoch] = clockCorrection;
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
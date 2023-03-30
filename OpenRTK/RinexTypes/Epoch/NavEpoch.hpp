#pragma once

#include "Epoch.hpp"
#include "../../RinexTypes/Satellite.hpp"
#include "../../NavData/NavData.hpp"
#include "../../NavData/GpsNavData.hpp"

#include <vector>

class NavEpoch : public Epoch
{
private:
	Satellite _Satellite;
	std::vector<NavData*> _NavigationData;

public:
	// getters
	Satellite const& SVs() const { return this->_Satellite;  }	
	std::vector<NavData*> const& NavigationData() const { return this->_NavigationData; }
	
	//setters
	std::vector<NavData*>& NavigationData() { return this->_NavigationData; }

	// ctor & dtor
	NavEpoch(int year, int month, int day, int hour, int minute, double second, Satellite satellite);
	~NavEpoch();	
};


#pragma once

#include "../../RinexParser/NavData/GalileoNavData.hpp"
#include "../../Transformations/ECEF_Position.hpp"

#include <math.h>
#include <memory>
#include <vector>

class Ephemeris
{
protected:
	ECEF_Position _Position_E;

public:
	// getters	
	ECEF_Position const& Position_E() const { return this->_Position_E; }

	// functions	
	virtual void CalcEphemeris(NavData& navData) = 0;			
};


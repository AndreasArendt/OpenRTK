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
	double _SatelliteClockError__s;
	double _RelativisticError__s;

public:
	// getters	
	ECEF_Position const& Position_E() const { return this->_Position_E; }
	double const& SatelliteClockError__s() const { return this->_SatelliteClockError__s;  }
	double const& RelativisticError__s() const { return this->_RelativisticError__s; }

	// functions	
	virtual void CalcEphemeris(NavData& navData) = 0;
	virtual std::unique_ptr<Ephemeris> clone() const = 0;

	// cotr & dtor
	Ephemeris() = default;	
	virtual ~Ephemeris() = default;

	// operator overloading
	Ephemeris& operator=(Ephemeris& rhs)
	{
		this->_Position_E = rhs.Position_E();
		return *this;
	}
};


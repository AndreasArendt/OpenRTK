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
	double _Utc__s;

public:
	// getters	
	ECEF_Position const& Position_E() const { return this->_Position_E; }
	double const& SatelliteClockError__s() const { return this->_SatelliteClockError__s;  }
	double const& RelativisticError__s() const { return this->_RelativisticError__s; }
	double const& Utc() const { return this->_Utc__s; }

	// functions	
	virtual void CalcEphemeris(NavData& navData, double time) = 0;
	virtual std::unique_ptr<Ephemeris> clone() const = 0;

	// cotr & dtor
	Ephemeris() : _SatelliteClockError__s(0), _RelativisticError__s(0), _Utc__s(0) {};
	virtual ~Ephemeris() = default;

	// operator overloading
	Ephemeris& operator=(Ephemeris& rhs)
	{
		this->_Position_E = rhs._Position_E;
		this->_RelativisticError__s = rhs._RelativisticError__s;
		this->_SatelliteClockError__s = rhs._SatelliteClockError__s;
		this->_Utc__s = rhs._Utc__s;

		return *this;
	}
};


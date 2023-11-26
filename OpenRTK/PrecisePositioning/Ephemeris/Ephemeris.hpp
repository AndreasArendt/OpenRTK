#pragma once

#include "../../Transformations/ECEF_Position.hpp"
#include "../../Transformations/ECEF_Velocity.hpp"
#include "../../RinexParser/NavData/NavData.hpp"
#include "../../RinexParser/NavData/SvHealth.hpp"

#include <math.h>
#include <memory>
#include <vector>

class Ephemeris
{
protected:
	ECEF_Position _Position_E;
	ECEF_Velocity _Velocity_E;
	SvHealth _SvHealth;	
	double _SatelliteClockError__s;
	double _RelativisticError__s;
	double _Utc__s;
	double _Toe__s;
	double _Obstime__s;

public:
	// getters	
	ECEF_Position const& Position_E() const { return this->_Position_E; }
	ECEF_Velocity const& Velocity_E() const { return this->_Velocity_E; }
	double const& SatelliteClockError__s() const { return this->_SatelliteClockError__s;  }
	double const& RelativisticError__s() const { return this->_RelativisticError__s; }
	double const& Utc() const { return this->_Utc__s; }
	double const& Toe__s() const { return this->_Toe__s; }
	double const& Obstime__s() const { return this->_Obstime__s; }
	SvHealth const& SatelliteHealth() const { return this->_SvHealth; }

	// functions	
	virtual void CalcEphemeris(NavData& navData, double time, double obstime) = 0;
	virtual double CalcClockOffset(NavData& navData, double time) = 0;
	virtual std::unique_ptr<Ephemeris> clone() const = 0;
		
	// cotr & dtor
	Ephemeris(SvHealth svHealth) : _SatelliteClockError__s(0), _RelativisticError__s(0), _Utc__s(0), _Toe__s(0), _Obstime__s(0), _SvHealth(svHealth)	{};

	virtual ~Ephemeris() = default;

	// operator overloading
	Ephemeris& operator=(Ephemeris& rhs);
};


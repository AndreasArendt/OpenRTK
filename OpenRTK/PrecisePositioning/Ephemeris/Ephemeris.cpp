#include "Ephemeris.hpp"

Ephemeris& Ephemeris::operator=(Ephemeris& rhs)
{
	this->_Position_E = rhs._Position_E;
	this->_Velocity_E = rhs._Velocity_E;
	this->_RelativisticError__s = rhs._RelativisticError__s;
	this->_SatelliteClockError__s = rhs._SatelliteClockError__s;
	this->_SatelliteClockDrift__1Ds = rhs._SatelliteClockDrift__1Ds;
	this->_Utc__s = rhs._Utc__s;
	this->_Toe__s = rhs._Toe__s;
	this->_Obstime__s = rhs._Obstime__s;

	return *this;
}
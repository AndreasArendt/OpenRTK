#include "Ephemeris.hpp"

Ephemeris& Ephemeris::operator=(Ephemeris& rhs)
{
	this->_Position_E = rhs._Position_E;
	this->_RelativisticError__s = rhs._RelativisticError__s;
	this->_SatelliteClockError__s = rhs._SatelliteClockError__s;
	this->_Utc__s = rhs._Utc__s;

	return *this;
}
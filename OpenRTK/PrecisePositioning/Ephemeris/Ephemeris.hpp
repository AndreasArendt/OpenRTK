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


#pragma once

#include "Ephemeris.hpp"
#include "../../RinexParser/NavData/Gps/GpsSvHealth.hpp"

class GpsEphemeris : virtual public Ephemeris
{
private:

public:
	// ctor & dtor	
	GpsEphemeris(GpsSvHealth svHealth);
	~GpsEphemeris();
};


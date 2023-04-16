#pragma once

#include "Ephemeris.hpp"

class GalileoEphemeris : virtual public Ephemeris
{
public:
	void CalcEphemeris(NavData& navData) override;

	// ctor & dtor
	GalileoEphemeris() {}
	~GalileoEphemeris() {}
};


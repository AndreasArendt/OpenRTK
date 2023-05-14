#pragma once

#include "Ephemeris.hpp"

#include <memory>

class GalileoEphemeris : virtual public Ephemeris
{
public:
	void CalcEphemeris(NavData& navData, double time) override;
	std::unique_ptr<Ephemeris> clone() const override;

	// ctor & dtor	
	GalileoEphemeris();	
	~GalileoEphemeris();
};


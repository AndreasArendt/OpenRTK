#pragma once

#include "Ephemeris.hpp"

#include <memory>

class GalileoEphemeris : virtual public Ephemeris
{   

private:
	double CalcMeanAnomaly(NavData& navData, double time);		

public:
	void CalcEphemeris(NavData& navData, double time, double obstime) override;
	void CalcClockOffset(NavData& navData, double time) override;
	std::unique_ptr<Ephemeris> clone() const override;

	// ctor & dtor	
	GalileoEphemeris(GalileoSvHealth svHealth);
	~GalileoEphemeris();
};


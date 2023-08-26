#pragma once

#include "Ephemeris.hpp"

#include "../../RinexParser/NavData/rtktypes.h"

#include <memory>

class GalileoEphemeris : virtual public Ephemeris
{   

private:
	double CalcMeanAnomaly(NavData& navData, double time);		
    double timediff(double t1, double t2);

public:
	void CalcEphemeris(NavData& navData, double time) override;
	void CalcClockOffset(NavData& navData, double time) override;
	std::unique_ptr<Ephemeris> clone() const override;
    void eph2pos(double time, const eph_t* eph, double* rs, double* dts, double* var);


	// ctor & dtor	
	GalileoEphemeris();	
	~GalileoEphemeris();
};


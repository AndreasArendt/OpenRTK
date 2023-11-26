#pragma once

#include "Velocity.hpp"

class ECEF_Velocity : public Velocity
{
private:
public:
	// ctor & dtor
	ECEF_Velocity() : Velocity() {}
	ECEF_Velocity(double x, double y, double z) : Velocity(x, y, z) {}
	~ECEF_Velocity() {}
};


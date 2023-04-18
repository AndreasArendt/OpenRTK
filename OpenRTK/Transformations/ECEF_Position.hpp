#pragma once

#include "Position.hpp"

class ECEF_Position : public Position
{
private:	
public:
	// ctor & dtor
	ECEF_Position() : Position() {}
	ECEF_Position(double x, double y, double z) : Position(x,y,z) {}
	~ECEF_Position() {}
};


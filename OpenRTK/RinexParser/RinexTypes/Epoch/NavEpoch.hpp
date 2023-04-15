#pragma once

#include "Epoch.hpp"

class NavEpoch : public Epoch
{
public:
	// ctor & dtor
	NavEpoch() = default;
	NavEpoch(int year, int month, int day, int hour, int minute, double second) : Epoch(year, month, day, hour, minute, second)
	{ }
	~NavEpoch() = default;		
};


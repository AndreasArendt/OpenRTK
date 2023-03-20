#pragma once

#include "Epoch.hpp"
#include "../../RinexTypes/Satellite.hpp"

class NavEpoch : public Epoch
{
private:
	Satellite _Satellite;

public:
	NavEpoch(int year, int month, int day, int hour, int minute, double second, Satellite satellite);
	~NavEpoch();	
};


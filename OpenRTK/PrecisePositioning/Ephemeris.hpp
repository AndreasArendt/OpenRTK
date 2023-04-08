#pragma once

#include "../RinexParser/NavData/NavData.hpp"
#include "../RinexParser/NavData/GalileoNavData.hpp"

#include <math.h>
#include <memory>
#include <vector>

class Ephemeris
{
private:
public:
	void CalcGalileoEphimeris(const std::vector<std::unique_ptr<NavData>>& navData);

	//ctor & dtor
	Ephemeris();
	~Ephemeris();
};


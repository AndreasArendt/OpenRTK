#pragma once

#include "../RinexParser/RinexTypes/Satellite.hpp"

#include <vector>

class SPP
{
public:
	// public functions
	void Calculate(std::vector<Satellite>& satellites);

	// ctor & dtor
	SPP();
	~SPP();
};


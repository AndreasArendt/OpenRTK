#pragma once

#include "../RinexParser/RinexTypes/Satellite.hpp"

class Postprocessor
{
private:
	bool CheckSatelliteHealthy(Satellite& sv);

public:

	void ShuffleData(std::vector<Satellite> satellites);

	// ctor & dtor
	Postprocessor();
	~Postprocessor();	
};


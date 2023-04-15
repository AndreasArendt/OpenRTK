#pragma once

#include "../RinexParser/NavData/GalileoNavData.hpp"
#include "../Transformations/ECEF_Position.hpp"

#include <math.h>
#include <memory>
#include <vector>

class Ephemeris
{
private:
	std::vector<ECEF_Position> _Position_E;

public:
	// getters	
	std::vector<ECEF_Position> const& Position_E() const { return this->_Position_E; }

	// functions
	void CalcGalileoEphimeris(const std::vector<std::unique_ptr<NavData>>& navData);

	//ctor & dtor
	Ephemeris();
	~Ephemeris();
};


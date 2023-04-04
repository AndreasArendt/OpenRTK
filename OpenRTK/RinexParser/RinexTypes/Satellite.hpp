#pragma once

#include "SvSystem.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris.hpp"
//#include "../RinexTypes/Epoch/Epoch.hpp"

#include <string>
#include <vector>

class Satellite
{

private:
	SvSystem _SvSystem;
	int _SvNumber;
	//std::vector<Epoch> _Epochs;
	Ephemeris _Ephemeris;

public:
	SvSystem const& SVSystem() const { return this->_SvSystem;  }
	int const& SvNumber() const { return this->_SvNumber; }
	//std::vector<Epoch> const& Epochs() const { return this->_Epochs; } 
	Ephemeris const& Ephemeris() const { return this->_Ephemeris; }
	
	// ctor & dtor
	Satellite();
	Satellite(SvSystem svSystem, int svNumber);
	Satellite(std::string satStr);
		
	~Satellite();

	// Operator overloading
	Satellite& operator=(const Satellite& other);
	bool operator==(const Satellite& other) const;
	bool operator!=(const Satellite& other) const;
};

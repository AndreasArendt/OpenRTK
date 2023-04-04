#pragma once

#include "SvSystem.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris.hpp"
#include "../RinexTypes/Epoch/Epoch.hpp"
#include "../RinexTypes/Epoch/NavEpoch.hpp"

#include <string>
#include <vector>

class Satellite
{

private:
	SvSystem _SvSystem;
	int _SvNumber;
	std::vector<std::unique_ptr<Epoch>> _Epochs;
	Ephemeris _Ephemeris;

public:
	// getters
	SvSystem const& SVSystem() const { return this->_SvSystem;  }
	int const& SvNumber() const { return this->_SvNumber; }
	std::vector<std::unique_ptr<Epoch>> const& Epochs() const { return this->_Epochs; } 
	Ephemeris const& Ephemeris() const { return this->_Ephemeris; }
	
	// public functions
	template <typename... Args, std::enable_if_t<std::is_constructible_v<NavEpoch, Args...>, int> = 0>
	void addNavEpoch(Args&&... args);
	
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

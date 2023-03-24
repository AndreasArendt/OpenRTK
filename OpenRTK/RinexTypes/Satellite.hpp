#pragma once

#include "SvSystem.hpp"
#include "../Utils/astring.hpp"

#include <string>

class Satellite
{

private:
	SvSystem _SvSystem;
	int _SvNumber;

public:
	SvSystem const& SVSystem() const { return this->_SvSystem;  }
	int const& SvNumber() const { return this->_SvNumber; }
	
	// ctor & dtor
	Satellite(SvSystem svSystem, int svNumber) : _SvSystem(svSystem), _SvNumber(svNumber) { }
	Satellite(std::string satStr)
	{
		this->_SvSystem = static_cast<SvSystem>(satStr.at(0));
		this->_SvNumber = parseInt(satStr.substr(1, 2));
	}

	~Satellite() { }
};

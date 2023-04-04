#pragma once

#include "SvSystem.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris.hpp"

#include <string>

class Satellite
{

private:
	SvSystem _SvSystem;
	int _SvNumber;
	Ephemeris _Ephemeris;

public:
	SvSystem const& SVSystem() const { return this->_SvSystem;  }
	int const& SvNumber() const { return this->_SvNumber; }
	Ephemeris const& Ephemeris() const { return this->_Ephemeris; }

	// ctor & dtor
	Satellite() : _SvSystem(SvSystem::UNKNOWN), _SvNumber(-1) { }
	Satellite(SvSystem svSystem, int svNumber) : _SvSystem(svSystem), _SvNumber(svNumber) { }
	Satellite(std::string satStr)
	{
		this->_SvSystem = static_cast<SvSystem>(satStr.at(0));
		this->_SvNumber = parseInt(satStr.substr(1, 2));
	}
		
	~Satellite() { }

	// Operator overloading
	Satellite& operator=(const Satellite& other)
	{
		if (this != &other)
		{
			_SvSystem = other.SVSystem();
			_SvNumber = other.SvNumber();
		}			

		return *this;		
	}

	bool operator==(const Satellite& other) const
	{
		return (this->_SvSystem == other.SVSystem()) && (this->_SvNumber == other.SvNumber());
	}

	bool operator!=(const Satellite& other) const	
	{
		return (this->_SvSystem != other.SVSystem()) || (this->_SvNumber != other.SvNumber());
	}
};

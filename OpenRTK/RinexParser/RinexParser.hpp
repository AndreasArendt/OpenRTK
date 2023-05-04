#pragma once

#include "RinexTypes/Satellite.hpp"

#include <string>
#include <vector>

class RinexParser
{
protected:
	std::vector<Satellite> _Satellites;

private:

	// private functions
	virtual void ParseLine(std::string line) = 0;

public:
	// getters
	std::vector<Satellite> const& Satellites() const { return this->_Satellites; }

	// functions
	virtual void ParseFile(std::string path) = 0;

	//ctor & dtor
	RinexParser();
	virtual ~RinexParser() {}
};


#pragma once

#include "AbstractRinexParser.hpp"

#include <vector>
#include <string>
#include <memory>

class RinexParser
{
private:
	std::vector<Satellite> _Satellites;
	std::unique_ptr<AbstractRinexParser> _RinexParser;

	// private function
	std::unique_ptr<AbstractRinexParser> GetParser(std::string path);

public:			
	// getters
	std::unique_ptr<AbstractRinexParser> const& RnxParser() const { return this->_RinexParser; }
	std::vector<Satellite> const& Satellites() const { return this->_Satellites; }

	// public functions
	void Parse(std::string path);

	//ctor & dtor
	~RinexParser() {}
	RinexParser() {}
};


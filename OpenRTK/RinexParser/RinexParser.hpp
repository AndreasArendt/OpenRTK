#pragma once

#include "AbstractRinexParser.hpp"

#include <string>
#include <memory>

class RinexParser
{
private:
	std::unique_ptr<AbstractRinexParser> _RinexParser;

	// private function
	std::unique_ptr<AbstractRinexParser> GetParser(std::string path);

public:			
	// getters
	std::unique_ptr<AbstractRinexParser> const& IsSpecialEvent() const { return this->_RinexParser; }

	// public functions
	void Parse(std::string path);

	//ctor & dtor
	~RinexParser() {}
	RinexParser() {}
};


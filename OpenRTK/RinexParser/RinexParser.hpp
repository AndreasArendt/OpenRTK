#pragma once

#include "AbstractRinexParser.hpp"

#include <string>
#include <memory>

class RinexParser
{
public:			
	std::unique_ptr<AbstractRinexParser> GetParser(std::string path);

	//ctor & dtor
	~RinexParser() {}
	RinexParser() {}
};


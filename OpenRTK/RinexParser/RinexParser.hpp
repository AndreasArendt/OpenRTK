#pragma once

#include "AbstractRinexParser.hpp"

#include <string>
#include <memory>

class RinexParser
{
private:	

public:		
	static std::unique_ptr<AbstractRinexParser> GetParser(std::string path);

	//ctor & dtor
	RinexParser() {}
};


#pragma once

#include "AbstractRinexParser.hpp"

#include <string>
#include <memory>

class RinexParser
{
private:	
	std::unique_ptr<AbstractRinexParser> GetParser(std::string path);

public:		
	void Parse(std::string path);

	//ctor & dtor
	~RinexParser() {}
	RinexParser() {}
};


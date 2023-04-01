#pragma once

#include <string>

class RinexParser
{
private:
	virtual void ParseLine(std::string line) = 0;

public:
	// functions
	virtual void ParseFile(std::string path);

	//ctor & dtor
	RinexParser();
	~RinexParser();
};


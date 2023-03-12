#pragma once

#include <string>

class RinexNavParser
{
private:
	void ParseLine(std::string line);

public:
	RinexNavParser();
	~RinexNavParser();

	void ParseFile(std::string path);
};


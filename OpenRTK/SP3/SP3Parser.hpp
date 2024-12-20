#pragma once

#include <string>

class SP3Parser
{
private:
	void ParseHeader(std::string& line);

public:
	void Parse(std::string& path);

	// ctor & dtor                
	SP3Parser();
	~SP3Parser();
};


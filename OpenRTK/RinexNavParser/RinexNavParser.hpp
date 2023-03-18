#pragma once

#include <string>

class RinexNavParser
{
private:
	std::string _Version;
	std::string _Type;

	void ParseLine(std::string line);

public:
	// getters
	std::string const& Version() const { return _Version; }	
	std::string const& Type() const { return _Type; }

	// ctor & dtor
	RinexNavParser();
	~RinexNavParser();

	// public function
	void ParseFile(std::string path);
};


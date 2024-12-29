#pragma once

#include "ClkParserState.hpp"
#include "ClkSatellite.hpp"

#include <vector>
#include <string>

class ClkParser
{
private:	
	ClkParserState _ClkParserState;
	std::string _Version;	
	std::vector<ClkSatellite> _Satellites;

	// functions
	void ParseHeader(std::string& line);
	void ParseSatelliteClockData(std::string& line);

public:
	// getters
	std::vector<ClkSatellite> const& Satellites() const { return this->_Satellites; }
	std::string const& Version() const { return this->_Version; }

	// functions
	void Parse(std::string& path);

	// ctor & dtor                
	ClkParser() : _ClkParserState(ClkParserState::PARSE_HEADER) {};
	~ClkParser() {};
};


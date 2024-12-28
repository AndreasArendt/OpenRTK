#pragma once

#include "ClkParserState.hpp"
#include "../Utils/AbstractSatellite.hpp"

#include <vector>
#include <string>

class ClkParser
{
private:
	std::vector<AbstractSatellite> _Satellites;
	ClkParserState _ClkParserState;
	std::string _Version;
	
	// functions
	void ParseHeader(std::string& line);

public:
	// getters
	std::vector<AbstractSatellite> const& Satellites() const { return this->_Satellites; }
	std::string const& Version() const { return this->_Version; }

	// functions
	void Parse(std::string& path);

	// ctor & dtor                
	ClkParser() : _ClkParserState(ClkParserState::PARSE_HEADER) {};
	~ClkParser() {};
};


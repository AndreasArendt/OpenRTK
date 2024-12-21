#pragma once

#include "SP3ParserState.hpp"
#include "SP3Satellite.hpp"

#include <string>
#include <vector>

class SP3Parser
{
private:
	static constexpr const int& NUMBER_SATELLITES_PER_HEADER_LINE = 17;

	SP3ParserState _Sp3ParserState;
	int _NumberSatellites;
	std::vector<SP3Satellite> _Satellites;

	void ParseHeader(std::string& line);
	void ParseDate(std::string& line);
	void ParseSatellites(std::string& line);

public:
	void Parse(std::string& path);

	// ctor & dtor                
	SP3Parser() : _Sp3ParserState(SP3ParserState::PARSE_HEADER), _NumberSatellites(-1) {};
	~SP3Parser() {};
};


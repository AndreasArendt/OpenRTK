#pragma once

#include "SP3ParserState.hpp"
#include "SP3Satellite.hpp"
#include "../Utils/Epoch.hpp"

#include <string>
#include <vector>

class SP3Parser
{
private:
	static constexpr const int& NUMBER_SATELLITES_PER_HEADER_LINE = 17;

	SP3ParserState _Sp3ParserState;
	int _NumberSatellites;
	std::vector<SP3Satellite> _Satellites;
	Epoch _CurrentEpoch;

	void ParseHeader(std::string& line);
	void ParseDate(std::string& line);
	void ParseSatellites(std::string& line);
	void ParseSatellitesAccuracy(std::string& line, int& lines_count);
	void ParseEpoch(std::string& line);
	void ParseEphemeris(std::string& line);

public:
	// getters
	std::vector<SP3Satellite> const& Satellites() const { return this->_Satellites; }

	// functions
	void Parse(std::string& path);

	// ctor & dtor                
	SP3Parser() : _Sp3ParserState(SP3ParserState::PARSE_HEADER), _NumberSatellites(-1) {};
	~SP3Parser() {};
};


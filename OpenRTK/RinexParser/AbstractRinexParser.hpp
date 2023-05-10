#pragma once

#include "RinexTypes/Satellite.hpp"

#include <string>

class AbstractRinexParser
{

protected:
	std::vector<Satellite>& _Satellites;

	// getters
	Satellite* const& CurrentSatellite() const { return this->_CurrentSatellite; }
		
	void FindCurrentSatellite(Satellite satellite);

private:
	Satellite* _CurrentSatellite = nullptr;

	virtual void InitParser() = 0;
	virtual void ParseLine(std::string line) = 0;

public:
	void Parse(std::string path);
		
	AbstractRinexParser(std::vector<Satellite>& satellites) : _Satellites(satellites) {};
	virtual ~AbstractRinexParser() = default;		
};
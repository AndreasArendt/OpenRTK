#include "AbstractRinexParser.hpp"

#include <fstream>
#include <iostream>

void AbstractRinexParser::FindCurrentSatellite(Satellite& satellite)
{
	auto it = std::find_if(this->_Satellites.begin(), this->_Satellites.end(), [&](Satellite& sv)
		{
			return satellite == sv;
		});

	if (it == this->_Satellites.end())
	{
		this->_Satellites.push_back(satellite);
		this->_CurrentSatellite = &this->_Satellites.back();
	}
	else
	{
		this->_CurrentSatellite = &(*it);
	}
}

void AbstractRinexParser::Parse(std::string path)
{
	std::ifstream infile(path);
	std::string firstline;
	std::getline(infile, firstline);
		
	this->InitParser();

	std::string line;
	while (std::getline(infile, line))
	{
		this->ParseLine(line);
		//std::cout << line << std::endl;
	}
}

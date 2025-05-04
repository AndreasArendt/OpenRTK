#include "AbstractRinexParser.hpp"

#include <fstream>
#include <iostream>

void AbstractRinexParser::FindCurrentSatellite(Satellite satellite)
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

void AbstractRinexParser::Parse(std::string path, bool verbose)
{
	std::ifstream file(path);
	if (!file)
		std::cout << "Could not open file.";

	this->InitParser();
		
	std::streampos totalSize = 0;
	if (verbose)
	{		
		file.seekg(0, std::ios::end);
		totalSize = file.tellg();
		file.seekg(0, std::ios::beg);
	}
		
	int lastReportedPercent = 0;
	std::string line;
	std::streampos lastPos = file.tellg();
	while (std::getline(file, line))
	{
		this->ParseLine(line);
		//std::cout << line << std::endl;

		std::streampos currentPos = file.tellg();
		if (currentPos == -1)
		{
			currentPos = totalSize;
		}
		if (verbose)
		{
			int percent = 100.0 * static_cast<double>(currentPos) / totalSize;
			if (percent - lastReportedPercent >= 1) 
			{
				std::cout << "\rParsing file: " << path << " - " << std::fixed << std::setprecision(0) << percent << "% completed" << std::flush;
				lastReportedPercent = percent;
			}
		}
	}

	if (verbose)
	{
		std::cout << std::endl;
	}
}

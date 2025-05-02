#include "RinexParser.hpp"
#include "RinexNavParser/RinexNavParser.hpp"
#include "RinexObsParser/RinexObsParser.hpp"

#include <fstream>
#include <iostream>

std::unique_ptr<AbstractRinexParser> RinexParser::GetParser(std::string path)
{	
	std::ifstream infile(path);
	std::string firstline;
	std::getline(infile, firstline);
		
	if ((firstline.find("NAVIGATION DATA") != std::string::npos) ||
		(firstline.find("NAV DATA") != std::string::npos))
	{		
		return std::make_unique<RinexNavParser>(this->_Satellites);		
	}
	else if (firstline.find("OBSERVATION DATA") != std::string::npos)
	{
		return std::make_unique<RinexObsParser>(this->_Satellites);
	}
	else
	{
		std::cout << "Unknown Filetype" << std::endl;
		return nullptr;
	}
}

void RinexParser::Parse(std::string path, bool verbose)
{
	if (verbose)
	{
		std::cout << "Parsing file: " << path << std::endl;
	}
	this->_RinexParser = this->GetParser(path);
	this->_RinexParser->Parse(path);
}

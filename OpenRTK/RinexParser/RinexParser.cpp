#include "RinexParser.hpp"
#include "RinexNavParser/RinexNavParser.hpp"
#include "RinexObsParser/RinexObsParser.hpp"

#include <fstream>
#include <iostream>
#include <memory>

void RinexParser::ParseFile(std::string path)
{
	std::ifstream infile(path);

	std::string firstline;
	std::getline(infile, firstline);

	std::unique_ptr<RinexParser> parser;

	if (firstline.find("NAVIGATION DATA") != std::string::npos)
	{
		parser = std::make_unique<RinexNavParser>();
	}
	else if (firstline.find("OBSERVATION DATA") != std::string::npos)
	{
		parser = std::make_unique<RinexObsParser>();
	}
	else
	{
		std::cout << "Unknown Filetype" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(infile, line))
	{
		this->ParseLine(line);
		//std::cout << line << std::endl;
	}
}

RinexParser::RinexParser()
{

}


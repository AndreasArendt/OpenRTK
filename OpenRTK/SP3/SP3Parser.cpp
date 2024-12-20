#include "SP3Parser.hpp"

#include "../Utils/astring.hpp"

#include <fstream>
#include <iostream>

void SP3Parser::ParseHeader(std::string& line)
{
	// #dP2024  7 18  0  0  0.00000000     289 ORBIT ITRF  BHN ESOC                    
	std::string version  = line.substr(1, 1);
	std::string fileType = line.substr(2, 1);
	
	// start date 
	int year = util::astring::parseInt(line.substr(3, 4));
	int month = util::astring::parseInt(line.substr(8, 2));
	int day = util::astring::parseInt(line.substr(11, 2));
	int hour = util::astring::parseInt(line.substr(14, 2));
	int min = util::astring::parseInt(line.substr(17, 2));
	double sec = util::astring::parseDouble(line.substr(20, 11));

	int noEpochs = util::astring::parseInt(line.substr(32, 7));
	std::string dataUsed = line.substr(40, 6);
	
	// ITRF : ITRF2000 https://www.ngs.noaa.gov/CORS/Articles/igs-refframe.pdf
	std::string coordinateSystem = line.substr(46, 5);
	std::string orbitType = line.substr(52, 3);
	std::string agency = line.substr(56, 4);
}

void SP3Parser::Parse(std::string& path)
{
	std::ifstream infile(path);
	if (!infile)
		std::cout << "Could not open file.";

	std::string firstline;
	std::getline(infile, firstline);

	this->ParseHeader(firstline);
}

SP3Parser::SP3Parser()
{
}

SP3Parser::~SP3Parser()
{

}


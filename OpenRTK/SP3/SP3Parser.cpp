#include "SP3Parser.hpp"

#include "../Utils/astring.hpp"

#include <fstream>
#include <iostream>
#include <cmath>

void SP3Parser::ParseHeader(std::string& line)
{
	// Specification version d https://files.igs.org/pub/data/format/sp3d.pdf
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

void SP3Parser::ParseDate(std::string& line)
{
	// TODO: implement
}

void SP3Parser::ParseSatellites(std::string& line)
{
	//+  117   G13G22G21G07G05G20G31G17G15G16G29G12G19G02G25G30G24      

	int idx_st = 9;

	for (size_t i = 0; i < SP3Parser::NUMBER_SATELLITES_PER_HEADER_LINE; i++)
	{	
		std::string sv_str = line.substr(idx_st + i * 3, 3);

		// stop when satellite empty
		if (sv_str.compare("  0") == 0)
			break;

		auto sat = SP3Satellite(sv_str);
		
		this->_Satellites.push_back(sat);
	}
}

void SP3Parser::Parse(std::string& path)
{
	std::ifstream infile(path);
	if (!infile)
		std::cout << "Could not open file.";
	
	int satellite_lines_cnt = 0;

	std::string line;
	while (std::getline(infile, line))
	{		
		//std::cout << line << std::endl;

		switch (this->_Sp3ParserState)
		{
		case SP3ParserState::PARSE_HEADER:
		{
			this->ParseHeader(line);
			this->_Sp3ParserState = SP3ParserState::PARSE_DATE;
			break;
		}
		case SP3ParserState::PARSE_DATE:
		{
			this->_Sp3ParserState = SP3ParserState::PARSE_SATELLITES;
			break;
		}
		case SP3ParserState::PARSE_SATELLITES:
		{
			// read first line
			if (satellite_lines_cnt == 0)
			{
				this->_NumberSatellites = util::astring::parseInt(line.substr(3, 3));
			}

			// check if all lines read
			if (std::ceil((double)this->_NumberSatellites / (double)SP3Parser::NUMBER_SATELLITES_PER_HEADER_LINE) == ++satellite_lines_cnt)
			{
				this->_Sp3ParserState = SP3ParserState::PARSE_SATELLITES_ACCURACY;
				satellite_lines_cnt = 0;
			}

			this->ParseSatellites(line);

			break;
		}
		case SP3ParserState::PARSE_SATELLITES_ACCURACY:
		{
			// check if all lines read
			if (std::ceil((double)this->_NumberSatellites / (double)SP3Parser::NUMBER_SATELLITES_PER_HEADER_LINE) == ++satellite_lines_cnt)
			{
				this->_Sp3ParserState = SP3ParserState::PARSE_SATELLITES_ACCURACY;
			}
			break;
		}
		default:
			break;
		}
	}

}


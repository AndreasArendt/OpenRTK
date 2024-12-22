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

void SP3Parser::ParseSatellitesAccuracy(std::string& line, int& lines_count)
{
	// lines_count is 1-based	
	//++         5  5  4  4  4  5  4  5  4  4  4  4  5  5  5  4  5                    	
	int idx_st = 9;

	for (size_t i = 0; i < SP3Parser::NUMBER_SATELLITES_PER_HEADER_LINE; i++)
	{
		std::string sv_str = line.substr(idx_st + i * 3, 3);

		// stop when satellite empty
		if (sv_str.compare("  0") == 0)
			break;
				
		this->_Satellites.at(i + (lines_count-1) * SP3Parser::NUMBER_SATELLITES_PER_HEADER_LINE).Accuracy(util::astring::parseInt(sv_str));				
	}
}

void SP3Parser::ParseEpoch(std::string& line)
{
	//*  2024  7 18  0  0  0.00000000		                      
	int year = util::astring::parseInt(line.substr(3, 4));
	int month = util::astring::parseInt(line.substr(8, 2));
	int day = util::astring::parseInt(line.substr(11, 2));
	int hour = util::astring::parseInt(line.substr(14, 2));
	int min = util::astring::parseInt(line.substr(17, 2));
	double sec = util::astring::parseDouble(line.substr(20, 11));

	this->_CurrentEpoch = Epoch(year, month, day, hour, min, sec);
}

void SP3Parser::ParseEphemeris(std::string& line)
{
	//PG13  17205.517550  13585.867440 -15376.740368    665.221919
	std::string type = line.substr(0, 1);
	auto sat = SP3Satellite(line.substr(1, 3));

	// position data - "V" would be velocity data
	if (type.compare("P") == 0)
	{
		double x = util::astring::parseDouble(line.substr(4, 14));
		double y = util::astring::parseDouble(line.substr(18, 14));
		double z = util::astring::parseDouble(line.substr(32, 14));

		double time = util::astring::parseDouble(line.substr(46, 15));

		auto it = std::find_if(this->_Satellites.begin(), this->_Satellites.end(), [&](SP3Satellite& sv)
			{
				return sv == sat;
			});

		if (it != this->_Satellites.end())
		{
			it->AddCorrectionData(this->_CurrentEpoch, ECEF_Position(x, y, z), time);
		}
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
				this->_Sp3ParserState = SP3ParserState::PARSE_WAIT_EPOCH;
			}

			this->ParseSatellitesAccuracy(line, satellite_lines_cnt);

			break;
		}
		case SP3ParserState::PARSE_WAIT_EPOCH:
		{
			if (line.substr(0, 1).compare("*") == 0)
			{
				this->ParseEpoch(line);
				this->_Sp3ParserState = SP3ParserState::PARSE_EPHEMERIS;
			}
			break;
		}
		case SP3ParserState::PARSE_EPHEMERIS:
		{
			if (line.substr(0, 1).compare("*") == 0)
			{
				this->ParseEpoch(line);
			}
			else if (line.substr(0, 3).compare("EOF") == 0)
			{
				this->_Sp3ParserState = SP3ParserState::PARSE_DONE;
			}
			else
			{
				this->ParseEphemeris(line);
			}
			break;
		}
		default:
			break;
		}
	}

}


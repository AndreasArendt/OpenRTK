#include "ClkParser.hpp"
#include "../Utils/astring.hpp"

#include <fstream>
#include <iostream>

void ClkParser::ParseHeader(std::string& line)
{
	//     3.02           C                                       RINEX VERSION / TYPE
	this->_Version = line.substr(5, 4);
}

void ClkParser::ParseSatelliteClockData(std::string& line)
{
	//AS G02  2024  7 18  0  0  0.000000  1   -0.397011722765E-03                     
	std::string clk_data_type = line.substr(0, 2);

	// check if the line is a satellite clock data
	if (clk_data_type.compare("AS") != 0)
	{
		return;
	}

	std::string sv_str = line.substr(3, 3);
	int year = util::astring::parseInt(line.substr(8, 4));
	int month = util::astring::parseInt(line.substr(13, 2));
	int day = util::astring::parseInt(line.substr(16, 2));
	int hour = util::astring::parseInt(line.substr(19, 2));
	int min = util::astring::parseInt(line.substr(22, 2));
	double sec = util::astring::parseDouble(line.substr(25, 9));
	int numberData = util::astring::parseInt(line.substr(35, 2));
	double clk_bias = util::astring::parseDouble(line.substr(40, 19));

	auto sat = ClkSatellite(sv_str);

	// check if the satellite is already exist
	auto it = std::find_if(this->_Satellites.begin(), this->_Satellites.end(), [&](ClkSatellite& sv)
		{
			return sv == sat;
		});

	// if exist, extend precise clock data
	if (it != this->_Satellites.end())
	{
		it->AddClockCorrectionData(Epoch(year, month, day, hour, min, sec), clk_bias);
	}
	else
	{
		sat.AddClockCorrectionData(Epoch(year, month, day, hour, min, sec), clk_bias);
		this->_Satellites.push_back(sat);
	}	
}

void ClkParser::Parse(std::string& path)
{
	std::ifstream infile(path);
	if (!infile)
		std::cout << "Could not open file.";

	int satellite_lines_cnt = 0;

	std::string line;
	while (std::getline(infile, line))
	{
		//std::cout << line << std::endl;

		switch (this->_ClkParserState)
		{
			case ClkParserState::PARSE_HEADER:
			{
				this->ParseHeader(line);
				this->_ClkParserState = ClkParserState::PARSE_WAIT_HEADER_END;
				break;
			}
			case ClkParserState::PARSE_WAIT_HEADER_END:
			{
				if (line.find("END OF HEADER") != std::string::npos)
				{
					this->_ClkParserState = ClkParserState::PARSE_SATELLITE_CLK_BIAS;
				}
				break;
			}
			case ClkParserState::PARSE_SATELLITE_CLK_BIAS:
			{
				this->ParseSatelliteClockData(line);
				break;
			}
			default:
				break;
		}
	}
}
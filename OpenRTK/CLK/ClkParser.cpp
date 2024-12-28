#include "ClkParser.hpp"
#include "../Utils/astring.hpp"

#include <fstream>
#include <iostream>

void ClkParser::ParseHeader(std::string& line)
{
	//     3.02           C                                       RINEX VERSION / TYPE
	this->_Version = line.substr(5, 4);

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
					this->_ClkParserState = ClkParserState::PARSE_SATELLITES;
				}
				break;
			}
			case ClkParserState::PARSE_SATELLITES:
			{
				break;
			}
			default:
				break;
		}
	}
}
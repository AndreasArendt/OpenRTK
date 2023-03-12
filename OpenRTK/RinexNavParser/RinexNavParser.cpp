#include "RinexNavParser.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

RinexNavParser::RinexNavParser()
{

}

RinexNavParser::~RinexNavParser()
{

}

void RinexNavParser::ParseLine(std::string line)
{

}

void RinexNavParser::ParseFile(std::string path)
{
    std::ifstream infile(path);

    std::string line;
    while (std::getline(infile, line))
    {
        this->ParseLine(line);
        std::cout << line << std::endl;
    }
}

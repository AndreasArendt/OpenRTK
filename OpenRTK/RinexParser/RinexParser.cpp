#include "RinexParser.hpp"

#include <fstream>

void RinexParser::ParseFile(std::string path)
{
    std::ifstream infile(path);
    
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

RinexParser::~RinexParser()
{

}


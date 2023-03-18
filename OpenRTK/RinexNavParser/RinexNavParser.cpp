#include "RinexNavParser.hpp"
#include "../Utils/strim.hpp"
#include "../RinexTypes/IonosphericCorrectionParameter.hpp"
#include "../RinexTypes/IonosphericCorrection.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

#define RINEX_VERSION_DEFINITION		  "RINEX VERSION / TYPE"
#define RINEX_IONOSPHERIC_CORR_DEFINITION "IONOSPHERIC CORR"

RinexNavParser::RinexNavParser()
{

}

RinexNavParser::~RinexNavParser()
{

}

void RinexNavParser::ParseLine(std::string line)
{
	if ((line.find(RINEX_VERSION_DEFINITION) != std::string::npos)) // read rinex file version
	{
		std::string ver = line.substr(0, 9);
		std::string type = line.substr(20, 1);
		this->_Version = ltrim(ver);
		this->_Type = ltrim(type);
	}
	else if (line.find(RINEX_IONOSPHERIC_CORR_DEFINITION) != std::string::npos)
	{
		std::string ionospheric_corr = line.substr(0, 4);
		IonosphericCorrectionParameter iono_param = IonosphericCorrectionParameterMap()[rtrim(ionospheric_corr)];
		auto ionosphericCorrection = std::make_unique< IonosphericCorrection>(iono_param);

		// extract ionospheric correction parameters
		double alphabeta0 = stod(line.substr(5, 12));
		double alphabeta1 = stod(line.substr(17, 12));
		double alphabeta2 = stod(line.substr(29, 12));
		double alphabeta3 = stod(line.substr(41, 12));		

		if (iono_param == GAL)
		{
			ionosphericCorrection->AddAlphaBeta(alphabeta0, alphabeta1, alphabeta2);
		}
		else
		{
			ionosphericCorrection->AddAlphaBeta(alphabeta0, alphabeta1, alphabeta2, alphabeta3);
		}
	}
}

void RinexNavParser::ParseFile(std::string path)
{
	std::ifstream infile(path);

	std::string line;
	while (std::getline(infile, line))
	{
		this->ParseLine(line);
		//std::cout << line << std::endl;
	}
}

#include "RinexNavParser.hpp"
#include "../Utils/strim.hpp"
#include "../RinexTypes/IonosphericCorrectionParameter.hpp"
#include "../RinexTypes/TimeDifferenceType.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

#define RINEX_VERSION_DEFINITION		  "RINEX VERSION / TYPE"
#define RINEX_IONOSPHERIC_CORR_DEFINITION "IONOSPHERIC CORR"
#define RINEX_TIME_SYSTEM_DIFF_DEFINITION "TIME SYSTEM CORR"
#define RINEX_END_OF_HEADER_DEFINITION    "END OF HEADER"

RinexNavParser::RinexNavParser()
{	
}

RinexNavParser::~RinexNavParser()
{
	this->_IonosphericCorrections.clear();
}

void RinexNavParser::ParseIonoCorrDefinition(std::string line)
{
	std::string ionospheric_corr = line.substr(0, 4);
	IonosphericCorrectionParameter iono_param = IonosphericCorrectionParameterMap()[rtrim(ionospheric_corr)];
	
	// extract ionospheric correction parameters
	double alphabeta0 = stod(line.substr(5, 12));
	double alphabeta1 = stod(line.substr(17, 12));
	double alphabeta2 = stod(line.substr(29, 12));
	double alphabeta3 = stod(line.substr(41, 12));
	char timeMark = line.at(54);

	if (iono_param == IonosphericCorrectionParameter::GAL)
	{	
		this->_IonosphericCorrections.emplace_back(iono_param, timeMark, alphabeta0, alphabeta1, alphabeta2);
	}
	else
	{
		this->_IonosphericCorrections.emplace_back(iono_param, timeMark, alphabeta0, alphabeta1, alphabeta2, alphabeta3);
	}		
}

void RinexNavParser::ParseTimeDiffDefinition(std::string line)
{
	std::string timeDiffType = line.substr(0, 4);
	TimeDifferenceType timeDifferenceType = TimeDifferenceTypeMap()[rtrim(timeDiffType)];
	
	double a0 = stod(line.substr(5, 17));
	double a1 = stod(line.substr(22, 16));
	int t = stoi(line.substr(39, 6));
	int w = stoi(line.substr(46, 4));
		
	this->_TimeSystemCorrections.emplace_back(timeDifferenceType, a0, a1, t, w);
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
		this->ParseIonoCorrDefinition(line);
	}
	else if (line.find(RINEX_TIME_SYSTEM_DIFF_DEFINITION) != std::string::npos)
	{
		this->ParseTimeDiffDefinition(line);
	}
	else if (line.find(RINEX_END_OF_HEADER_DEFINITION) != std::string::npos)
	{
		this->_RinexHeaderParsed = true;
	}
	else if (_RinexHeaderParsed)
	{
		// Parse Epochs
		//this->_Epochs.emplace_back();
		double a0 = stod(line.substr(5, 17));

	}
}

void RinexNavParser::ParseFile(std::string path)
{
	std::ifstream infile(path);
	this->_RinexHeaderParsed = false;

	std::string line;
	while (std::getline(infile, line))
	{
		this->ParseLine(line);
		//std::cout << line << std::endl;
	}
}

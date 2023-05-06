#include "RinexNavParser.hpp"
#include "../../Utils/strim.hpp"
#include "../../Utils/astring.hpp"
#include "../RinexTypes/IonosphericCorrectionParameter.hpp"
#include "../RinexTypes/TimeDifferenceType.hpp"
#include "../NavData/GpsNavData.hpp"
#include "../NavData/GalileoNavData.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

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
	this->_IonosphericCorrections.clear();
	this->_TimeSystemCorrections.clear();		
}

void RinexNavParser::ParseIonoCorrDefinition(std::string line)
{
	std::string ionospheric_corr = line.substr(0, 4);
	IonosphericCorrectionParameter iono_param = IonosphericCorrectionParameterMap()[rtrim(ionospheric_corr)];

	// extract ionospheric correction parameters
	double alphabeta0 = parseDouble(line.substr(5, 12));
	double alphabeta1 = parseDouble(line.substr(17, 12));
	double alphabeta2 = parseDouble(line.substr(29, 12));
	double alphabeta3 = parseDouble(line.substr(41, 12));
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

	double a0 = parseDouble(line.substr(5, 17));
	double a1 = parseDouble(line.substr(22, 16));
	int t = parseInt(line.substr(39, 6));
	int w = parseInt(line.substr(46, 4));

	this->_TimeSystemCorrections.emplace_back(timeDifferenceType, a0, a1, t, w);
}

void RinexNavParser::ParseOrbitData(std::string line)
{
	// writing this explicitly for clarity
	switch (this->_CurrentOrbitNumber)
	{
	case ENavOrbitNumber::ORBIT_1:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_2;
		break;
	case ENavOrbitNumber::ORBIT_2:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_3;
		break;
	case ENavOrbitNumber::ORBIT_3:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_4;
		break;
	case ENavOrbitNumber::ORBIT_4:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_5;
		break;
	case ENavOrbitNumber::ORBIT_5:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_6;
		break;
	case ENavOrbitNumber::ORBIT_6:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_7;
		break;
	case ENavOrbitNumber::ORBIT_7:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_UNKNOWN; // this will be reset in parsing function
		break;
	default:
		this->_CurrentOrbitNumber = ENavOrbitNumber::ORBIT_1;
		break;
	}

	double data0 = parseDouble(line.substr(4, 19));
	double data1 = parseDouble(line.substr(23, 19));
	double data2 = parseDouble(line.substr(42, 19));
	double data3 = parseDouble(line.substr(61, 19));

	this->_CurrentNavData->AddOrbit(this->_CurrentOrbitNumber, data0, data1, data2, data3);
}

void RinexNavParser::ParseEpoch(std::string line)
{
	switch (_NavEpochParsingState)
	{
	case NavEPochParsingState_SKIP:
	{
		// stay in SKIP state until line start is non-empty
		if (line.at(0) == ' ')
		{
			break;
		}
	}
	case NavEpochParsingState_IDLE:
	case NavEpochParsingState_CLOCK_ERROR:
	{		
		// Parse Epochs
		auto satellite = Satellite(line.substr(0, 3));
		int year = parseInt(line.substr(4, 4));
		int month = parseInt(line.substr(9, 2));
		int day = parseInt(line.substr(12, 2));
		int hour = parseInt(line.substr(15, 2));
		int minute = parseInt(line.substr(18, 2));
		double second = parseInt(line.substr(21, 2));
		
		// parse SV clock bias (seconds), SV clock drift (sec/sec) and SV clock drift rate (sec/sec2)
		double clockBias = parseDouble(line.substr(23, 19));
		double clockDrift = parseDouble(line.substr(42, 19));
		double clockDriftRate = parseDouble(line.substr(61, 19));
				
		// GET CURRENT SATTELITE
		this->FindCurrentSatellite(satellite);

		switch (this->CurrentSatellite()->SVSystem())
		{
		case SvSystem::GPS:
		{
			this->_CurrentNavData = std::make_unique<GpsNavData>(year, month, day, hour, minute, second);
			this->_CurrentNavData->AddClockErrors(clockBias, clockDrift, clockDriftRate);

			this->_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT;
			break;
		}
		case SvSystem::GALILEO:
		{
			this->_CurrentNavData = std::make_unique<GalileoNavData>(year, month, day, hour, minute, second);
			this->_CurrentNavData->AddClockErrors(clockBias, clockDrift, clockDriftRate);

			this->_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT;
			break;
		}
		case SvSystem::GLONASS:
		case SvSystem::GZSS:
		case SvSystem::BEIDOU:
		case SvSystem::NAVIC_IRNSS:
		case SvSystem::SBAS:
		case SvSystem::MIXED:
		case SvSystem::UNKNOWN:
		default:
		{
			this->_NavEpochParsingState = NavEpochParsingState::NavEPochParsingState_SKIP;
			break;
		}
		}

		break;
	}
	case NavEpochParsingState_ORBIT:
	{
		this->ParseOrbitData(line);

		if (this->_CurrentOrbitNumber == ENavOrbitNumber::ORBIT_UNKNOWN)
		{
			this->CurrentSatellite()->addNavData(std::move(this->_CurrentNavData));
			this->_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;
		}
		break;
	}
	default:
	{
		this->_NavEpochParsingState = NavEpochParsingState::NavEPochParsingState_SKIP;
		this->_CurrentNavData.reset();
		break;
	}
	}
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
		this->ParseEpoch(line);
	}
}

void RinexNavParser::InitParser()
{
	this->_RinexHeaderParsed = false;	
}
#include "RinexNavParser.hpp"
#include "../Utils/strim.hpp"
#include "../Utils/astring.hpp"
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
		switch (_NavEpochParsingState)
		{
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

				this->_Epochs.emplace_back(year, month, day, hour, minute, second, satellite);

				// parse SV clock bias (seconds), SV clock drift (sec/sec) and SV clock drift rate (sec/sec2)
				double clockBias = parseDouble(line.substr(23, 19));
				double clockDrift = parseDouble(line.substr(42, 19));
				double clockDriftRate = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddClockErrors(clockBias, clockDrift, clockDriftRate);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_1;
				break;
			}
			case NavEpochParsingState_ORBIT_1:
			{
				double IODE_IssueOfData = parseDouble(line.substr(4, 19));
				double Crs__m = parseDouble(line.substr(23, 19));
				double DeltaN__radDs = parseDouble(line.substr(42, 19));
				double M0__rad = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_1(IODE_IssueOfData, Crs__m, DeltaN__radDs, M0__rad);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_2;
				break;
			}
			case NavEpochParsingState_ORBIT_2:
			{
				double Cuc__rad = parseDouble(line.substr(4, 19));
				double Eccentricity = parseDouble(line.substr(23, 19));
				double Cus__rad = parseDouble(line.substr(42, 19));
				double SqrtA___sqrtm = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_2(Cuc__rad, Eccentricity, Cus__rad, SqrtA___sqrtm);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_3;
				break;
			}
			case NavEpochParsingState_ORBIT_3:
			{
				double Toe__s = parseDouble(line.substr(4, 19));
				double Cic__rad = parseDouble(line.substr(23, 19));
				double Omega0__rad = parseDouble(line.substr(42, 19));
				double Cis__rad = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_3(Toe__s, Cic__rad, Omega0__rad, Cis__rad);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_4;
				break;
			}
			case NavEpochParsingState_ORBIT_4:
			{
				double i0__rad = parseDouble(line.substr(4, 19));
				double Crc__m = parseDouble(line.substr(23, 19));
				double Omega__rad = parseDouble(line.substr(42, 19));
				double Omega_dot__radDs = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_4(i0__rad, Crc__m, Omega__rad, Omega_dot__radDs);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_5;
				break;
			}
			case NavEpochParsingState_ORBIT_5:
			{
				double Idot__radDs = parseDouble(line.substr(4, 19));
				double CodesOnL2 = parseDouble(line.substr(23, 19));
				double GpsWeek = parseDouble(line.substr(42, 19));
				double L2P_DataFlag = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_5(Idot__radDs, CodesOnL2, GpsWeek, L2P_DataFlag);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_6;
				break;
			}
			case NavEpochParsingState_ORBIT_6:
			{
				double SvAccuracy__m = parseDouble(line.substr(4, 19));
				double SvHealth = parseDouble(line.substr(23, 19));
				double TGD__s = parseDouble(line.substr(42, 19));
				double IODC = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_6(SvAccuracy__m, SvHealth, TGD__s, IODC);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_ORBIT_7;
				break;
			}
			case NavEpochParsingState_ORBIT_7:
			{
				double TransmissiontimeOfMessage = parseDouble(line.substr(4, 19));
				double FitInterval__hrs = parseDouble(line.substr(23, 19));
				double Spare0 = parseDouble(line.substr(42, 19));
				double Spare1 = parseDouble(line.substr(61, 19));

				this->_Epochs.back().AddOrbit_7(TransmissiontimeOfMessage, FitInterval__hrs, Spare0, Spare1);
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;
				break;
			}
			default:
			{
				_NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;
				break;
			}
		}
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

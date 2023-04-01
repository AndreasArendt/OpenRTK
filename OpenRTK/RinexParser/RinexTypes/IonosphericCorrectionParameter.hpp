#pragma once

#include <map>
#include <string>

enum IonosphericCorrectionParameter
{
	IONOPARAM_UNKNOWN = 0,
	GAL,	// Galileo AI0-AI2
	GPSA,	// GPS Alpha0-Alpha3
	GPSB,   // GPS beta0-beta3
	QZSA,   // QZS alpha0-alpha3
	QZSB,   // GZS beta0-beta3
	BDSA,   // BDS alpha0-alpha3
	BDSB,   // BDS beta0-beta3
	IRNA,   // NavIC/IRNSS alpha0-alpha3
	IRNB    // NavIC/IRNSS beta0-beta3
};

// String switch paridgam   
struct IonosphericCorrectionParameterMap : public std::map<std::string, IonosphericCorrectionParameter>
{
	IonosphericCorrectionParameterMap()
	{
		this->operator[]("GAL") = GAL;
		this->operator[]("GPSA") = GPSA;
		this->operator[]("GPSB") = GPSB;
		this->operator[]("QZSA") = QZSA;
		this->operator[]("QZSB") = QZSB;
		this->operator[]("BDSA") = BDSA;
		this->operator[]("BDSB") = BDSB;
		this->operator[]("IRNA") = IRNA;
		this->operator[]("IRNB") = IRNB;		
	};
	~IonosphericCorrectionParameterMap() {}
};
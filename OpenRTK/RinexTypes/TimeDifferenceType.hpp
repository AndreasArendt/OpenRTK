#pragma once

#include <map>
#include <string>

enum TimeDifferenceType
{
	TDIFF_UNKNOWN = 0,
	GPUT,	// GPS - UTC (a0, a1)
	GLUT,	// GLO - UTC (a0 = -TauC, a1 = zero)
	GAUT,	// GAL - UTC (a0, a1)
	BDUT,	// BDS - UTC (a0 = A0_UTC, a1 = A1_UTC)
	QZUT,	// QZS - UTC (a0, a1)
	IRUT,	// IRN - UTC (a0 = A0_UTC, a1 = A1_UTC)
	SBUT,	// SBAS - UTC (a0, a1)
	GLGP,	// GLO - GPS (a0 = -TauGPS, a1 = zero)
	GAGP,	// GAL - GPS
	GZGP,	// GZS - GPS (a0, a1)
	IRGP	// IRN - GPS (a0=A0, a2=A1)
};

// String switch paridgam   
struct TimeDifferenceTypeMap : public std::map<std::string, TimeDifferenceType>
{
	TimeDifferenceTypeMap()
	{
		this->operator[]("GPUT") = GPUT;
		this->operator[]("GLUT") = GLUT;
		this->operator[]("GAUT") = GAUT;
		this->operator[]("BDUT") = BDUT;
		this->operator[]("QZUT") = QZUT;
		this->operator[]("IRUT") = IRUT;
		this->operator[]("SBUT") = SBUT;
		this->operator[]("GLGP") = GLGP;
		this->operator[]("GAGP") = GAGP;
		this->operator[]("GZGP") = GZGP;
		this->operator[]("IRGP") = IRGP;
	};
	~TimeDifferenceTypeMap() {}
};
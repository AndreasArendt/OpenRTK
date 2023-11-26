#pragma once

#include "../../RinexParser/NavData/NavData.hpp"
#include "../../Transformations/ECEF_Position.hpp"
#include "../../Transformations/ECEF_Velocity.hpp"

struct KeplerOrbitData
{
	double SqrtA___sqrtm;	
	double DeltaN__radDs;
	double M0__rad;
	double Eccentricity;
	double Omega__rad;
	double Omega0__rad;
	double Omega_dot__radDs;
	double Cus__rad;
	double Cuc__rad;
	double Crs__m;
	double Crc__m;
	double Cis__rad;
	double Cic__rad;
	double i0__rad;
	double Idot__radDs;
	double ToeEpoch;
	double Toe__s;	
};

class KeplerOrbit
{
private:	
	double _RelativisticError__s;

	double CalcMeanAnomaly(KeplerOrbitData& orbitData, double time);

public:
	std::tuple<ECEF_Position, ECEF_Velocity> CalcEphemeris(KeplerOrbitData& orbitData, double time, double obstime);
	
	// ctor & dtor
	KeplerOrbit();
	~KeplerOrbit();
};
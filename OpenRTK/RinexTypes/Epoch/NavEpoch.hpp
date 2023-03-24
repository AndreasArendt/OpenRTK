#pragma once

#include "Epoch.hpp"
#include "../../RinexTypes/Satellite.hpp"

class NavEpoch : public Epoch
{
private:
	Satellite _Satellite;
	double _SV_ClockBias__s;
	double _SV_ClockDrift__sDs;
	double _SV_ClockDriftRate__sDs2;

	//Orbit 1
	double _IODE_IssueOfData;
	double _Crs__m;
	double _DeltaN__radDs;
	double _M0__rad;

	// Orbit 2
	double _Cuc__rad;
	double _Eccentricity;
	double _Cus__rad;
	double _SqrtA___sqrtm;

	// Orbit 3
	double _Toe__s; //Time of Ephemeris
	double _Cic__rad;
	double _Omega0__rad;
	double _Cis__rad;

	// Orbit 4
	double _i0__rad;
	double _Crc__m;
	double _Omega__rad;
	double _Omega_dot__radDs;

	// Orbit 5
	double _Idot__radDs;
	double _CodesOnL2;
	double _GpsWeek; //continuous, nod Mod(1024)!
	double _L2P_DataFlag;

	// Orbit 6
	double _SvAccuracy__m;
	double _SvHealth;
	double _TGD__s;
	double _IODC;
	
	// Orbit 7
	double _TransmissiontimeOfMessage;
	double _FitInterval__hrs;
	double _Spare0;
	double _Spare1;

public:
	// getters

	// functions
	void AddClockErrors(double SV_ClockBias__s, double SV_ClockDrift__sDs, double SV_ClockDriftRate__sDs2);
	void AddOrbit_1(double IODE_IssueOfData, double Crs__m, double DeltaN__radDs, double M0__rad);
	void AddOrbit_2(double Cuc__rad, double Eccentricity, double Cus__rad, double SqrtA___sqrtm);
	void AddOrbit_3(double Toe__s, double Cic__rad, double Omega0__rad, double Cis__rad);
	void AddOrbit_4(double i0__rad, double Crc__m, double Omega__rad, double Omega_dot__radDs);
	void AddOrbit_5(double Idot__radDs, double CodesOnL2, double GpsWeek, double L2P_DataFlag);
	void AddOrbit_6(double SvAccuracy__m, double SvHealth, double TGD__s, double IODC);
	void AddOrbit_7(double TransmissiontimeOfMessage, double FitInterval__hrs, double Spare0, double Spare1);

	// ctor & dtor
	NavEpoch(int year, int month, int day, int hour, int minute, double second, Satellite satellite);
	~NavEpoch();	
};


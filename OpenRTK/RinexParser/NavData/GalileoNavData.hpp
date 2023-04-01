#pragma once

#include "NavData.hpp"

class GalileoNavData : virtual public NavData
{
private:
	// Clock Data
	double _SV_ClockBias__s = 0.0;
	double _SV_ClockDrift__sDs = 0.0;
	double _SV_ClockDriftRate__sDs2 = 0.0;

	//Orbit 1
	double _IODE_IssueOfData = 0.0;
	double _Crs__m = 0.0;
	double _DeltaN__radDs = 0.0;
	double _M0__rad = 0.0;

	// Orbit 2
	double _Cuc__rad = 0.0;
	double _Eccentricity = 0.0;
	double _Cus__rad = 0.0;
	double _SqrtA___sqrtm = 0.0;
	
	// Orbit 3
	double _Toe__s = 0.0; //Time of Ephemeris
	double _Cic__rad = 0.0;
	double _Omega0__rad = 0.0;
	double _Cis__rad = 0.0;

	// Orbit 4
	double _i0__rad = 0.0;
	double _Crc__m = 0.0;
	double _Omega__rad = 0.0;
	double _Omega_dot__radDs = 0.0;

	// Orbit 5
	double _Idot__radDs = 0.0;
	double _DataSources = 0.0;
	double _GalWeek = 0.0;
	double _Spare0 = 0.0;

	// Orbit 6
	double _SigAccuracy__m = 0.0;
	double _SvHealth = 0.0;
	double _BDG_E5a_E1 = 0.0;
	double _BDG_E5b_E1 = 0.0;

	// Orbit 7		
	double _TransmissiontimeOfMessage = 0.0;
	double _Spare1 = 0.0;
	double _Spare2 = 0.0;
	double _Spare3 = 0.0;

public:
	// Geerbt über NavData
	void AddClockErrors(double data0, double data1, double data2);
	void AddOrbit_1(double data0, double data1, double data2, double data3);
	void AddOrbit_2(double data0, double data1, double data2, double data3);
	void AddOrbit_3(double data0, double data1, double data2, double data3);
	void AddOrbit_4(double data0, double data1, double data2, double data3);
	void AddOrbit_5(double data0, double data1, double data2, double data3);
	void AddOrbit_6(double data0, double data1, double data2, double data3);
	void AddOrbit_7(double data0, double data1, double data2, double data3);

	NavData* clone() const override
	{
		return new GalileoNavData(*this);
	}

	GalileoNavData();
	GalileoNavData(const GalileoNavData& other) = default;
	~GalileoNavData();
};


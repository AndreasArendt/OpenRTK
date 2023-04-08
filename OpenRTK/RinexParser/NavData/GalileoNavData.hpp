#pragma once

#include "NavData.hpp"

#include <memory>

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

protected:
	void AddOrbit_1(double data0, double data1, double data2, double data3);
	void AddOrbit_2(double data0, double data1, double data2, double data3);
	void AddOrbit_3(double data0, double data1, double data2, double data3);
	void AddOrbit_4(double data0, double data1, double data2, double data3);
	void AddOrbit_5(double data0, double data1, double data2, double data3);
	void AddOrbit_6(double data0, double data1, double data2, double data3);
	void AddOrbit_7(double data0, double data1, double data2, double data3);

public:	
	// Clock Data
	double const& SV_ClockBias__s() const { return _SV_ClockBias__s; }
	double const& SV_ClockDrift__sDs() const { return _SV_ClockDrift__sDs; }
	double const& SV_ClockDriftRate__sDs2() const { return _SV_ClockDriftRate__sDs2; }

	// Orbit 1
	double const& IODE_IssueOfData() const { return _IODE_IssueOfData; }
	double const& Crs__m() const { return _Crs__m; }
	double const& DeltaN__radDs() const { return _DeltaN__radDs; }
	double const& M0__rad() const { return _M0__rad; }

	// Orbit 2
	double const& Cuc__rad() const { return _Cuc__rad; }
	double const& Eccentricity() const { return _Eccentricity; }
	double const& Cus__rad() const { return _Cus__rad; }
	double const& SqrtA___sqrtm() const { return _SqrtA___sqrtm; }

	// Orbit 3
	double const& Toe__s() const { return _Toe__s; }
	double const& Cic__rad() const { return _Cic__rad; }
	double const& Omega0__rad() const { return _Omega0__rad; }
	double const& Cis__rad() const { return _Cis__rad; }

	// Orbit 4
	double const& i0__rad() const { return _i0__rad; }
	double const& Crc__m() const { return _Crc__m; }
	double const& Omega__rad() const { return _Omega__rad; }
	double const& Omega_dot__radDs() const { return _Omega_dot__radDs; }

	// Orbit 5
	double const& Idot__radDs() const { return _Idot__radDs; }
	double const& DataSources() const { return _DataSources; }
	double const& GalWeek() const { return _GalWeek; }
	
	// Orbit 6
	double const& SigAccuracy__m() const { return _SigAccuracy__m; }
	double const& SvHealth() const { return _SvHealth; }
	double const& BDG_E5a_E1() const { return _BDG_E5a_E1; }
	double const& BDG_E5b_E1() const { return _BDG_E5b_E1; }

	// Orbit 7
	double TransmissiontimeOfMessage() const { return _TransmissiontimeOfMessage; }
	
	void AddClockErrors(double data0, double data1, double data2);

	std::unique_ptr<NavData> clone() const override;

	GalileoNavData();		
	~GalileoNavData();
};


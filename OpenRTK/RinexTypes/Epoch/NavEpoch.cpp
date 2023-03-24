#include "NavEpoch.hpp"

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second, Satellite satellite) : Epoch(year, month, day, hour, minute, second), _Satellite(satellite)
{

}

void NavEpoch::AddClockErrors(double SV_ClockBias__s, double SV_ClockDrift__sDs, double SV_ClockDriftRate__sDs2)
{
	_SV_ClockBias__s = SV_ClockBias__s;
	_SV_ClockDrift__sDs = SV_ClockDrift__sDs;
	_SV_ClockDriftRate__sDs2 = SV_ClockDriftRate__sDs2;
}

void NavEpoch::AddOrbit_1(double IODE_IssueOfData, double Crs__m, double DeltaN__radDs, double M0__rad)
{
	_IODE_IssueOfData = IODE_IssueOfData;
	_Crs__m = Crs__m;
	_DeltaN__radDs = DeltaN__radDs;
	_M0__rad = M0__rad;
}

void NavEpoch::AddOrbit_2(double Cuc__rad, double Eccentricity, double Cus__rad, double SqrtA___sqrtm)
{
	_Cuc__rad = Cuc__rad;
	_Eccentricity = Eccentricity;
	_Cus__rad = Cus__rad;
	_SqrtA___sqrtm = SqrtA___sqrtm;
}

void NavEpoch::AddOrbit_3(double Toe__s, double Cic__rad, double Omega0__rad, double Cis__rad)
{
	_Toe__s = Toe__s;
	_Cic__rad = Cic__rad;
	_Omega0__rad = Omega0__rad;
	_Cis__rad = Cis__rad;
}

void NavEpoch::AddOrbit_4(double i0__rad, double Crc__m, double Omega__rad, double Omega_dot__radDs)
{
	_i0__rad = i0__rad;
	_Crc__m = Crc__m;
	_Omega__rad = Omega__rad;
	_Omega_dot__radDs = Omega_dot__radDs;
}

void NavEpoch::AddOrbit_5(double Idot__radDs, double CodesOnL2, double GpsWeek, double L2P_DataFlag)
{
	_Idot__radDs = Idot__radDs;
	_CodesOnL2 = CodesOnL2;
	_GpsWeek = GpsWeek;
	_L2P_DataFlag = L2P_DataFlag;
}

void NavEpoch::AddOrbit_6(double SvAccuracy__m, double SvHealth, double TGD__s, double IODC)
{
	_SvAccuracy__m = SvAccuracy__m;
	_SvHealth = SvHealth;
	_TGD__s = TGD__s;
	_IODC = IODC;
}

void NavEpoch::AddOrbit_7(double TransmissiontimeOfMessage, double FitInterval__hrs, double Spare0, double Spare1)
{
	_TransmissiontimeOfMessage = TransmissiontimeOfMessage;
	_FitInterval__hrs = FitInterval__hrs;
	_Spare0 = Spare0;
	_Spare1 = Spare1;
}

NavEpoch::~NavEpoch()
{

}


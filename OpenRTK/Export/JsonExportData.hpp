#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../Transformations/ECEF_Position.hpp"
#include "../Utils/Epoch.hpp"
#include "../RinexParser/NavData/Galileo/GalileoNavData.hpp"
#include "../RinexParser/NavData/Gps/GpsNavData.hpp"

class PreciseClock
{
public:
	std::string SatelliteSystem;
	double SatelliteClockError__s;

	nlohmann::json to_json()
	{
		return
		{
			{"SatelliteSystem", this->SatelliteSystem},
			{"SatelliteClockError__s", this->SatelliteClockError__s}
		};
	}

	PreciseClock() : SatelliteSystem(""), SatelliteClockError__s(0) { }
};

class PreciseEphemeris
{
public:
	std::string SatelliteSystem;
	ECEF_Position ECEF_Position;
	double SatelliteClockError__us;
	double Accuracy;

	nlohmann::json to_json()
	{
		return
		{
			{"SatelliteSystem", this->SatelliteSystem},
			{"Position_E", {{"x", this->ECEF_Position.x()}, {"y", this->ECEF_Position.y()}, {"z", this->ECEF_Position.z()}}},
			{"SatelliteClockError__us", this->SatelliteClockError__us},
			{"Accuracy", this->Accuracy}
		};
	}

	PreciseEphemeris() : SatelliteSystem(""), ECEF_Position(0, 0, 0), SatelliteClockError__us(0), Accuracy(0) { }
};

class GenericObservation
{
public:
	double Band_1;
	double Band_2;
	double Band_5;

	nlohmann::json to_json()
	{
		return
		{
			{"Band_1", this->Band_1},
			{"Band_2", this->Band_2},
			{"Band_5", this->Band_5}
		};
	}

	GenericObservation() : Band_1(0), Band_2(0), Band_5(0) { }
};

class SatelliteObservation
{
public:
	std::string SatelliteSystem;
	ECEF_Position ECEF_Position;
	double ClockOffset;
	double ClockDrift;
	double RelativisticError;

	bool IsHealthy;

	GenericObservation Code;
	GenericObservation Carrier;
	GenericObservation Doppler;
	GenericObservation Snr;

	nlohmann::json to_json()
	{
		return {
		   {"SatelliteSystem", this->SatelliteSystem},
		   {"ECEF_Position", {{"x", this->ECEF_Position.x()}, {"y", this->ECEF_Position.y()}, {"z", this->ECEF_Position.z()}}},
		   {"ClockOffset", this->ClockOffset},
		   {"ClockDrift", this->ClockDrift},
		   {"RelativisticError", this->RelativisticError},
		   {"IsHealthy", this->IsHealthy},
		   {"Code", this->Code.to_json()},
		   {"Carrier", this->Carrier.to_json()},
		   {"Doppler", this->Doppler.to_json()},
		   {"Snr", this->Snr.to_json()}
		};
	}
};

class SatelliteData
{
public:
	double PosixEpochTime__s;
	std::vector<SatelliteObservation> Observations;
};

class PreciseEphemerisData
{
public:
	double PosixEpochTime__s;
	std::vector<PreciseEphemeris> PreciseEphemeris;
};

class PreciseClockData
{
public:
	double PosixEpochTime__s;
	std::vector<PreciseClock> PreciseClock;
};

static class JGpsNavDataConvert
{
public:
	static nlohmann::json to_json(const GpsNavData& const navData)
	{
		return {
			{"PosixEpochTime__s", navData.Epoche().PosixEpochTime__s()},
			{"SV_ClockBias__s", navData.SV_ClockBias__s()},
			{"SV_ClockDrift__sDs", navData.SV_ClockDrift__sDs()},
			{"SV_ClockDriftRate__sDs2", navData.SV_ClockDriftRate__sDs2()},
			{"IODE_IssueOfData", navData.IODE_IssueOfData()},
			{"Crs__m", navData.Crs__m()},
			{"DeltaN__radDs", navData.DeltaN__radDs()},
			{"M0__rad", navData.M0__rad()},
			{"Cuc__rad", navData.Cuc__rad()},
			{"Eccentricity", navData.Eccentricity()},
			{"Cus__rad", navData.Cus__rad()},
			{"SqrtA___sqrtm", navData.SqrtA___sqrtm()},
			{"Toe__s", navData.Toe__s()},
			{"Cic__rad", navData.Cic__rad()},
			{"Omega0__rad", navData.Omega0__rad()},
			{"Cis__rad", navData.Cis__rad()},
			{"i0__rad", navData.i0__rad()},
			{"Crc__m", navData.Crc__m()},
			{"Omega__rad", navData.Omega__rad()},
			{"Omega_dot__radDs", navData.Omega_dot__radDs()},
			{"Idot__radDs", navData.Idot__radDs()},
			{"CodesOnL2", navData.CodesOnL2()},
			{"GpsWeek", navData.GpsWeek()},
			{"L2P_DataFlag", navData.L2P_DataFlag()},
			{"SvAccuracy__m", navData.SvAccuracy__m()},
			{"SvHealth", navData.SvHealth()},
			{"TGD__s", navData.TGD__s()},
			{"IODC", navData.IODC()},
			{"TransmissiontimeOfMessage",navData.TransmissiontimeOfMessage()},
			{"FitInterval__hrs", navData.FitInterval__hrs()},
			{"Spare0", navData.Spare0()},
			{"Spare1", navData.Spare1()}
		};
	}
};

static class JGalileoNavDataConvert
{
public:
	static nlohmann::json to_json(const GalileoNavData& const navData)
	{
		return {
			{"PosixEpochTime__s", navData.Epoche().PosixEpochTime__s()},
			{"SV_ClockBias__s", navData.SV_ClockBias__s()},
			{"SV_ClockDrift__sDs", navData.SV_ClockDrift__sDs()},
			{"SV_ClockDriftRate__sDs2", navData.SV_ClockDriftRate__sDs2()},
			{"IODE_IssueOfData", navData.IODE_IssueOfData()},
			{"Crs__m", navData.Crs__m()},
			{"DeltaN__radDs", navData.DeltaN__radDs()},
			{"M0__rad", navData.M0__rad()},
			{"Cuc__rad", navData.Cuc__rad()},
			{"Eccentricity", navData.Eccentricity()},
			{"Cus__rad", navData.Cus__rad()},
			{"SqrtA___sqrtm", navData.SqrtA___sqrtm()},
			{"Toe__s", navData.Toe__s()},
			{"Cic__rad", navData.Cic__rad()},
			{"Omega0__rad", navData.Omega0__rad() },
			{"Cis__rad", navData.Cis__rad() },
			{"i0__rad", navData.i0__rad() },
			{"Crc__m", navData.Crc__m()},
			{"Omega__rad", navData.Omega__rad() },
			{"Omega_dot__radDs", navData.Omega_dot__radDs() },
			{"Idot__radDs", navData.Idot__radDs() },
			{"DataSources", navData.DataSources()},
			{"GalWeek", navData.GalWeek() },
			{"Spare0", navData.Spare0() },
			{"SigAccuracy__m", navData.SigAccuracy__m() },
			{"SvHealth", navData.SvHealth()},
			{"BDG_E5a_E1", navData.BDG_E5a_E1() },
			{"BDG_E5b_E1", navData.BDG_E5b_E1() },
			{"TransmissiontimeOfMessage", navData.TransmissiontimeOfMessage() },
			{"Spare1", navData.Spare1()},
			{"Spare2", navData.Spare2() },
			{"Spare3", navData.Spare3() }
		};
	}
};
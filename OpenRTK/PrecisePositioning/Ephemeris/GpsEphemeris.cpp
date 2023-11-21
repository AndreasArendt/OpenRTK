#include "GpsEphemeris.hpp"
#include "../../RinexParser/NavData/Gps/GpsNavData.hpp"
#include "../../RinexParser/NavData/SvHealth.hpp"
#include "../Ephemeris/KeplerOrbit.hpp"

GpsEphemeris::GpsEphemeris(GpsSvHealth svHealth) : Ephemeris(static_cast<SvHealth>(svHealth))
{
}

GpsEphemeris::~GpsEphemeris()
{
}

void GpsEphemeris::CalcClockOffset(NavData& navData, double time)
{
	auto nav = dynamic_cast<GpsNavData&>(navData);

	// apply clock correction - taken from RTKLIB eph2clk
	double t = time - nav.Epoche().Toc__s();
	double ts = t;

	for (int i = 0; i < 2; i++)
	{
		t = ts - (nav.SV_ClockBias__s() + nav.SV_ClockDrift__sDs() * t + nav.SV_ClockDriftRate__sDs2() * t * t);
	}

	this->_SatelliteClockError__s = nav.SV_ClockBias__s() + nav.SV_ClockDrift__sDs() * t + nav.SV_ClockDriftRate__sDs2() * t * t;
}

void GpsEphemeris::CalcEphemeris(NavData& navData, double time, double obstime)
{
	auto nav = dynamic_cast<GpsNavData&>(navData);

	// Satellite Clock Offset Correction
	this->CalcClockOffset(navData, time);
	time = time - this->_SatelliteClockError__s;

	KeplerOrbitData orbitData =
	{
		.SqrtA___sqrtm = nav.SqrtA___sqrtm(),
		.DeltaN__radDs = nav.DeltaN__radDs(),
		.M0__rad = nav.M0__rad(),
		.Eccentricity = nav.Eccentricity(),
		.Omega__rad = nav.Omega__rad(),
		.Omega0__rad = nav.Omega0__rad(),
		.Omega_dot__radDs = nav.Omega_dot__radDs(),
		.Cus__rad = nav.Cus__rad(),
		.Cuc__rad = nav.Cuc__rad(),
		.Crs__m = nav.Crs__m(),
		.Crc__m = nav.Crc__m(),
		.Cis__rad = nav.Cis__rad(),
		.Cic__rad = nav.Cic__rad(),
		.i0__rad = nav.i0__rad(),
		.Idot__radDs = nav.Idot__radDs(),
		.ToeEpoch = nav.ToeEpoch(),
		.Toe__s = nav.Toe__s()
	};

	auto orbit = KeplerOrbit();
	this->_Position_E = orbit.CalcEphemeris(orbitData, time, obstime);

	this->_Utc__s = time;
	this->_Toe__s = nav.ToeEpoch();
	this->_Obstime__s = obstime;
}

void GpsEphemeris::CalcVelocity()
{

}

std::unique_ptr<Ephemeris> GpsEphemeris::clone() const
{
	return std::make_unique<GpsEphemeris>(*this);
}

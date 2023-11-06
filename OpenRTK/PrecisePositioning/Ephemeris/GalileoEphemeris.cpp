#include "GalileoEphemeris.hpp"
#include "../../Transformations/Transformation.hpp"
#include "../../Transformations/Constants.hpp"

#include <cmath>
#include <chrono>

#include <iostream>

GalileoEphemeris::GalileoEphemeris(GalileoSvHealth svHealth) : Ephemeris(svHealth)
{
}

GalileoEphemeris::~GalileoEphemeris()
{
}

void GalileoEphemeris::CalcClockOffset(NavData& navData, double time)
{
	auto nav = dynamic_cast<GalileoNavData&>(navData);

	// apply clock correction - taken from RTKLIB eph2clk
	double t = time - nav.Toc__s();
	double ts = t;

	for (int i = 0; i < 2; i++)
	{
		t = ts - (nav.SV_ClockBias__s() + nav.SV_ClockDrift__sDs() * t + nav.SV_ClockDriftRate__sDs2() * t * t);
	}

	this->_SatelliteClockError__s = nav.SV_ClockBias__s() + nav.SV_ClockDrift__sDs() * t + nav.SV_ClockDriftRate__sDs2() * t * t;	
}

double GalileoEphemeris::CalcMeanAnomaly(NavData& navData, double time)
{	
	auto nav = dynamic_cast<GalileoNavData&>(navData);	
	
	double t_k = time - nav.ToeEpoch();

	// 1 semi circle = pi rad	
	double A = nav.SqrtA___sqrtm() * nav.SqrtA___sqrtm();

	// Computed mean motion (rad/s)
	double n_0 = sqrt(Transformation::GravitationalConstant__m3Ds2 / pow(A, 3));

	// Corrected mean motion
	double n = n_0 + nav.DeltaN__radDs();

	// Mean anomaly at reference time
	double M = nav.M0__rad() + n * t_k;

	double E = M; // initial guess
	double dE = 1;
	int iters = 0;
	while ((std::abs(dE) > 1e-13) && (iters < 100))
	{
		dE = (E - nav.Eccentricity() * std::sin(E) - M) / (1 - nav.Eccentricity() * std::cos(E));
		E -= dE;
		iters++;
	}

	return E;
}

void GalileoEphemeris::CalcEphemeris(NavData& navData, double time, double obstime)
{
	// Satellite Clock Offset Correction
	this->CalcClockOffset(navData, time);
	time = time - this->_SatelliteClockError__s;
	
	auto nav = dynamic_cast<GalileoNavData&>(navData);	
	
	// Semo major Axis
	double A = nav.SqrtA___sqrtm() * nav.SqrtA___sqrtm();
	
	// Mean Anomaly
	double E = this->CalcMeanAnomaly(navData, time);

	// Relativistic Error Correction	
	double F = -2 * sqrt(Transformation::GravitationalConstant__m3Ds2) / (Transformation::SpeedOfLight__mDs * Transformation::SpeedOfLight__mDs);
	this->_RelativisticError__s = F * nav.Eccentricity() * nav.SqrtA___sqrtm() * sin(E);
	time = time - this->_RelativisticError__s;

	// Calc Mean Anomaly with corrected time again
	E = this->CalcMeanAnomaly(navData, time);
	
	double t_k = time - nav.ToeEpoch();

	// true anomaly
	double sqrt_1_e2 = sqrt(1 - nav.Eccentricity() * nav.Eccentricity());
	double sinE = sin(E);
	double minus_ecosE_plus_1 = -nav.Eccentricity() * cos(E) + 1;

	double cosE_minus_E = cos(E) - nav.Eccentricity();

	double v = atan2(sqrt_1_e2 * (sinE / minus_ecosE_plus_1), cosE_minus_E / minus_ecosE_plus_1);

	// Argument of Latitude (phi)
	double phi = v + nav.Omega__rad();

	// argument of Latitude Correction
	double delta_u = nav.Cus__rad() * sin(2 * phi) + nav.Cuc__rad() * cos(2 * phi);

	// radius correction
	double delta_r = nav.Crs__m() * sin(2 * phi) + nav.Crc__m() * cos(2 * phi);

	// inclination correction
	double delta_i = nav.Cis__rad() * sin(2 * phi) + nav.Cic__rad() * cos(2 * phi);

	// Corrected Argument of Latitude
	double u = phi + delta_u;

	// Corrected radius
	double r = A * (1 - nav.Eccentricity() * cos(E)) + delta_r;

	// Corrected inclination
	double i = nav.i0__rad() + delta_i + nav.Idot__radDs() * t_k;

	// Position in orbital plane
	double x_prime = r * cos(u);
	double y_prime = r * sin(u);

	// Corrected langitude of ascending node	
	double OMEGA = nav.Omega0__rad() + (nav.Omega_dot__radDs() - Transformation::MeanAngularVelocityOfEarth__radDs) * t_k - Transformation::MeanAngularVelocityOfEarth__radDs * nav.Toe__s();

	// GTRF coordinates of the SV antenna phase center position at time t	
	double x = x_prime * cos(OMEGA) - y_prime * cos(i) * sin(OMEGA);
	double y = x_prime * sin(OMEGA) + y_prime * cos(i) * cos(OMEGA);
	double z = y_prime * sin(i);

	this->_Position_E = ECEF_Position(x, y, z);
	this->_Utc__s = time;
	this->_Toe__s = nav.ToeEpoch();
	this->_Obstime__s = obstime;
}

std::unique_ptr<Ephemeris> GalileoEphemeris::clone() const
{
	return std::make_unique<GalileoEphemeris>(*this);
}

#include "GalileoEphemeris.hpp"
#include "../../Transformations/Transformation.hpp"
#include "../../Transformations/Constants.hpp"

#include <cmath>
#include <chrono>

#include <iostream>

GalileoEphemeris::GalileoEphemeris() : Ephemeris() 
{
}

GalileoEphemeris::~GalileoEphemeris()
{
}

void GalileoEphemeris::CalcEphemeris(NavData& navData)
{
	auto nav = dynamic_cast<GalileoNavData&>(navData);
	auto trans = Transformation();
		
	// 1 semi circle = pi rad
	double M0__semiCircles			= nav.M0__rad()			 / Constants::pi;
	double delta_n__semiCirclesDs	= nav.DeltaN__radDs()	 / Constants::pi;
	double OMEGA_0__cemiCircles		= nav.Omega0__rad()		 / Constants::pi;
	double i0__semiCirclesDs	    = nav.i0__rad()			 / Constants::pi;
	double Omega__semiCircles		= nav.Omega__rad()	     / Constants::pi;
	double OMEGA_dot__semiCirclesDs = nav.Omega_dot__radDs() / Constants::pi;
	double i_dot__semiCirclesDs     = nav.Idot__radDs()      / Constants::pi;

	// Semo major Axis
	double A = nav.SqrtA___sqrtm() * nav.SqrtA___sqrtm();

	// Computed mean motion (rad/s)
	double n_0 = sqrt(trans.GravitationalConstant__m3Ds2 / pow(A, 3));

	//Time from ephermeris reference poch
	double t = nav.getReceiverTime(); // t := TOC TODO: correction by signal transmission duration missing!	
	double t_k = t - nav.Toe__s();

	if (t_k > 302400)
	{
		t_k -= 604800;
	}
	else if (t_k < -302400)
	{
		t_k += 604800;
	}
	
	// Corrected mean motion
	double n = n_0 + delta_n__semiCirclesDs;

	// Mean anomaly
	double M = M0__semiCircles + n * t_k;

	double E = M; // initial guess
	double dE = 1;
	int iters = 0;	
	while ((std::abs(dE) > 1e-13) && (iters < 100))
	{
		dE = (E - nav.Eccentricity() * std::sin(E) - M) / (1 - nav.Eccentricity() * std::cos(E));
		E -= dE;
		iters++;
	}
		
	// clocl offsets
	double delta_t_sv = nav.SV_ClockBias__s() + nav.SV_ClockDrift__sDs() * t_k + nav.SV_ClockDriftRate__sDs2() * t_k * t_k;
	double F = -2 * sqrt(trans.GravitationalConstant__m3Ds2) / (trans.SpeedOfLight__mDs * trans.SpeedOfLight__mDs);
	double delta_t_relativistic = F * nav.Eccentricity() * nav.SqrtA___sqrtm() * sin(E);

	// true anomaly
	double sqrt_1_e2 = sqrt(1 - nav.Eccentricity() * nav.Eccentricity());
	double sinE = sin(E);
	double minus_ecosE_plus_1 = -nav.Eccentricity() * cos(E) + 1;

	double cosE_minus_E = cos(E) - nav.Eccentricity();

	double v = atan2(sqrt_1_e2 * (sinE / minus_ecosE_plus_1), cosE_minus_E / minus_ecosE_plus_1);

	// Argument of Latitude (phi)
	double phi = v + Omega__semiCircles;

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
	double i = i0__semiCirclesDs + delta_i + i_dot__semiCirclesDs * t_k;

	// Position in orbital plane
	double x_prime = r * cos(u);
	double y_prime = r * sin(u);

	// Corrected langitude of ascending node
	double OMEGA = OMEGA_0__cemiCircles + (OMEGA_dot__semiCirclesDs - trans.MeanAngularVelocityOfEarth__radDs) * t_k - trans.MeanAngularVelocityOfEarth__radDs * nav.Toe__s();

	// GTRF coordinates of the SV antenna phase center position at time t	
	double x = x_prime * cos(OMEGA) - y_prime * cos(i) * sin(OMEGA);
	double y = x_prime * sin(OMEGA) + y_prime * cos(i) * cos(OMEGA);
	double z = y_prime * sin(i);

	this->_Position_E = ECEF_Position(x, y, z);
}

std::unique_ptr<Ephemeris> GalileoEphemeris::clone() const
{
	return std::make_unique<GalileoEphemeris>(*this);
}

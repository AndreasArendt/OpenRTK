#include "KeplerOrbit.hpp"

#include "../../Transformations/Transformation.hpp"

KeplerOrbit::KeplerOrbit()
{

}

KeplerOrbit::~KeplerOrbit()
{

}

double KeplerOrbit::CalcMeanAnomaly(KeplerOrbitData& orbitData, double time)
{	
	double t_k = time - orbitData.ToeEpoch;

	// 1 semi circle = pi rad	
	double A = orbitData.SqrtA___sqrtm * orbitData.SqrtA___sqrtm;

	// Computed mean motion (rad/s)
	double n_0 = sqrt(Transformation::GravitationalConstant__m3Ds2 / pow(A, 3));

	// Corrected mean motion
	double n = n_0 + orbitData.DeltaN__radDs;

	// Mean anomaly at reference time
	double M = orbitData.M0__rad + n * t_k;

	double E = M; // initial guess
	double dE = 1;
	int iters = 0;
	while ((std::abs(dE) > 1e-13) && (iters < 100))
	{
		dE = (E - orbitData.Eccentricity * std::sin(E) - M) / (1 - orbitData.Eccentricity * std::cos(E));
		E -= dE;
		iters++;
	}

	return E;
}

std::tuple<ECEF_Position, ECEF_Velocity> KeplerOrbit::CalcEphemeris(KeplerOrbitData& orbitData, double time, double obstime)
{
	// Semi major Axis
	double A = orbitData.SqrtA___sqrtm * orbitData.SqrtA___sqrtm;

	// Mean Anomaly
	double E = this->CalcMeanAnomaly(orbitData, time);

	// Relativistic Error Correction	
	double F = -2 * sqrt(Transformation::GravitationalConstant__m3Ds2) / (Transformation::SpeedOfLight__mDs * Transformation::SpeedOfLight__mDs);
	this->_RelativisticError__s = F * orbitData.Eccentricity * orbitData.SqrtA___sqrtm * sin(E);
	time = time - this->_RelativisticError__s;

	// Calc Mean Anomaly with corrected time again
	E = this->CalcMeanAnomaly(orbitData, time);

	double t_k = time - orbitData.ToeEpoch;

	// true anomaly
	double sqrt_1_e2 = sqrt(1 - orbitData.Eccentricity * orbitData.Eccentricity);
	double sinE = sin(E);
	double minus_ecosE_plus_1 = -orbitData.Eccentricity * cos(E) + 1;

	double cosE_minus_E = cos(E) - orbitData.Eccentricity;

	double v = atan2(sqrt_1_e2 * (sinE / minus_ecosE_plus_1), cosE_minus_E / minus_ecosE_plus_1);

	// Argument of Latitude (phi)
	double phi = v + orbitData.Omega__rad;

	// argument of Latitude Correction
	double delta_u = orbitData.Cus__rad * sin(2 * phi) + orbitData.Cuc__rad * cos(2 * phi);

	// radius correction
	double delta_r = orbitData.Crs__m * sin(2 * phi) + orbitData.Crc__m * cos(2 * phi);

	// inclination correction
	double delta_i = orbitData.Cis__rad * sin(2 * phi) + orbitData.Cic__rad * cos(2 * phi);

	// Corrected Argument of Latitude
	double u = phi + delta_u;

	// Corrected radius
	double r = A * (1 - orbitData.Eccentricity * cos(E)) + delta_r;

	// Corrected inclination
	double i = orbitData.i0__rad + delta_i + orbitData.Idot__radDs * t_k;

	// Position in orbital plane
	double x_prime = r * cos(u);
	double y_prime = r * sin(u);

	// Corrected langitude of ascending node	
	double OMEGA = orbitData.Omega0__rad + (orbitData.Omega_dot__radDs - Transformation::MeanAngularVelocityOfEarth__radDs) * t_k - Transformation::MeanAngularVelocityOfEarth__radDs * orbitData.Toe__s;

	// GTRF coordinates of the SV antenna phase center position at time t	
	double x = x_prime * cos(OMEGA) - y_prime * cos(i) * sin(OMEGA);
	double y = x_prime * sin(OMEGA) + y_prime * cos(i) * cos(OMEGA);
	double z = y_prime * sin(i);

	// ==================================================
	// Calculate Velocity
	// ==================================================		

	// Computed mean motion (rad/s)
	double n_0 = sqrt(Transformation::GravitationalConstant__m3Ds2 / pow(A, 3));

	// Corrected mean motion
	double n = n_0 + orbitData.DeltaN__radDs;

	double Ek_dot = n / (1 - orbitData.Eccentricity * cos(E)); // (1)

	// True Anomaly rate
	double vk_dot = (Ek_dot * sqrt(1 - orbitData.Eccentricity * orbitData.Eccentricity)) / (1 - orbitData.Eccentricity * cos(E)); // (2)

	// Corrected Inclination Angle Rate
	double dldot_kDdt = orbitData.Idot__radDs + 2 * vk_dot * (orbitData.Cis__rad * cos(2 * phi) - orbitData.Cic__rad * sin(2 * phi)); // (3)

	// Corrected Argument of Latitude Rate
	double u_dot = vk_dot + 2 * vk_dot * (orbitData.Cus__rad * cos(2 * phi) - orbitData.Cuc__rad * sin(2 * phi)); // (4)

	double r_dot = orbitData.Eccentricity * A * Ek_dot * sin(E) +
		2 * vk_dot * (orbitData.Crs__m * cos(2 * phi) - orbitData.Crc__m * sin(2 * phi)); // (5)

	double Omegak_dot = orbitData.Omega_dot__radDs - Transformation::MeanAngularVelocityOfEarth__radDs; // (6)

	// In plane x,y velocity
	double x_prime_dot = r_dot * cos(u) - r * u_dot * sin(u); // (7)
	double y_prime_dot = r_dot * sin(u) + r * u_dot * cos(u); // (8)
	
	// ECEF Velocity
	double xdot = -x_prime * Omegak_dot * sin(OMEGA) + x_prime_dot * cos(OMEGA) -
		y_prime_dot * sin(OMEGA) * cos(i) -
		y_prime * (Omegak_dot * cos(OMEGA) * cos(i) - dldot_kDdt * sin(OMEGA) * sin(i));

	double ydot = x_prime * Omegak_dot * cos(OMEGA) +
		x_prime_dot * sin(OMEGA) +
		y_prime_dot * cos(OMEGA) * cos(i) -
		y_prime * (Omegak_dot * sin(OMEGA) * cos(i) + dldot_kDdt * cos(OMEGA) * sin(i));

	double zdot = y_prime_dot * sin(i) + y_prime * dldot_kDdt * cos(i);

	return std::tuple<ECEF_Position, ECEF_Velocity>(ECEF_Position(x, y, z), ECEF_Velocity(xdot, ydot, zdot));
}
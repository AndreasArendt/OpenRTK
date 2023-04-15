#include "Ephemeris.hpp"
#include "../Transformations/Transformation.hpp"

#include <cmath>
#include <chrono>

void Ephemeris::CalcGalileoEphimeris(const std::vector<std::unique_ptr<NavData>>& navData)
{
	auto nav = dynamic_cast<GalileoNavData*>(navData.begin()->get());
	auto trans = Transformation();

	for (const auto& pnavData : navData)
	{
		GalileoNavData* nav = dynamic_cast<GalileoNavData*>(pnavData.get());
		
		double t = nav->getGST();

		// Semo major Axis
		double A = nav->SqrtA___sqrtm() * nav->SqrtA___sqrtm();

		// Computed mean motion (rad/s)
		double n_0 = sqrt(trans.GravitationalConstant__m3Ds2 / pow(A, 3));

		//Time from ephermeris reference poch
		double t_k = t - nav->Toe__s();

		// Corrected mean motion
		double n = n_0 + nav->DeltaN__radDs();

		// Mean anomaly
		double M = nav->M0__rad() * n * t_k;

		double E = M; // initial guess
		double dE = 1;
		while (std::abs(dE) > 1e-12)
		{
			dE = (E - nav->Eccentricity() * std::sin(E) - M) / (1 - nav->Eccentricity() * std::cos(E));
			E -= dE;
		}

		// true anomaly
		double sqrt_1_e2 = sqrt(1 - nav->Eccentricity() * nav->Eccentricity());
		double sinE = sin(E);
		double minus_ecosE_plus_1 = -nav->Eccentricity() * cos(E) + 1;

		double cosE_minus_E = cos(E) - nav->Eccentricity();

		double v = atan2(sqrt_1_e2 * (sinE / minus_ecosE_plus_1), cosE_minus_E / minus_ecosE_plus_1);

		// Argument of Latitude (phi)
		double phi = v + nav->Omega__rad();

		// argument of Latitude Correction
		double delta_u = nav->Cus__rad() * sin(2 * phi) + nav->Cuc__rad() * cos(2 * phi);

		// radius correction
		double delta_r = nav->Crs__m() * sin(2 * phi) + nav->Crc__m() * cos(2 * phi);

		// inclination correction
		double delta_i = nav->Cis__rad() * sin(2 * phi) + nav->Cic__rad() * cos(2 * phi);

		// Corrected Argument of Latitude
		double u = phi + delta_u;

		// Corrected radius
		double r = A * (1 - nav->Eccentricity() * cos(E)) + delta_r;

		// Corrected inclination
		double i = nav->i0__rad() + delta_i + nav->Idot__radDs() * t_k;

		// Position in orbital plane
		double x_prime = r * cos(u);
		double y_prime = r * sin(u);

		// Corrected langitude of ascending node
		double OMEGA = nav->Omega0__rad() + (nav->Omega_dot__radDs() - trans.MeanAngularVelocityOfEarth__radDs) * t_k - trans.MeanAngularVelocityOfEarth__radDs * nav->Toe__s();

		// GTRF coordinates of the SV antenna phase center position at time t	
		double x = x_prime * cos(OMEGA) - y_prime * cos(i) * sin(OMEGA);
		double y = x_prime * sin(OMEGA) + y_prime * cos(i) * cos(OMEGA);
		double z = y_prime * sin(i);

		this->_Position_E.emplace_back(x, y, z);
	}
}

Ephemeris::Ephemeris()
{

}

Ephemeris::~Ephemeris()
{
}
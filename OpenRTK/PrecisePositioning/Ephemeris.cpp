#include "Ephemeris.hpp"

void Ephemeris::CalcGalileoEphimeris(const std::vector<std::unique_ptr<NavData>>& navData)
{	
	auto nav = dynamic_cast<GalileoNavData*>(navData.begin()->get());

	// Semi-Major Axis
	double A = nav->SqrtA___sqrtm() * nav->SqrtA___sqrtm();
}

Ephemeris::Ephemeris()
{

}

Ephemeris::~Ephemeris()
{
}
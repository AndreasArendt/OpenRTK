#include "NavEpoch.hpp"

NavEpoch::NavEpoch() : Epoch()
{

}

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second) : Epoch(year, month, day, hour, minute, second)
{
	
}

void NavEpoch::addSatellite(const Satellite& sv)
{
	this->_Satellites.emplace_back(sv);
}

NavEpoch::~NavEpoch()
{
	_Satellites.clear();
}

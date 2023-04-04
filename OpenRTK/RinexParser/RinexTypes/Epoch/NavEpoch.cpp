#include "NavEpoch.hpp"

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second) : Epoch(year, month, day, hour, minute, second)
{
	
}

NavEpoch::~NavEpoch()
{
	_NavigationData.clear();
}


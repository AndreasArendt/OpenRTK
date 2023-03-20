#include "NavEpoch.hpp"

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second, Satellite satellite) : Epoch(year, month, day, hour, minute, second), _Satellite(satellite)
{

}

NavEpoch::~NavEpoch()
{

}


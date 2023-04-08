#include "NavEpoch.hpp"

NavEpoch::NavEpoch() : Epoch()
{

}

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second) : Epoch(year, month, day, hour, minute, second)
{
	
}

Satellite* NavEpoch::TryAddSatellite(const Satellite& sv)
{
    auto it = std::find_if(this->_Satellites.begin(), this->_Satellites.end(), [&](const Satellite& sat)
        {
            return sat == sv;
        });

    if (it == this->_Satellites.end())
    {
        this->_Satellites.push_back(sv);
        return &this->_Satellites.back();
    }
    else
    {
        return &(*it);
    }
}

NavEpoch::~NavEpoch()
{
	_Satellites.clear();
}

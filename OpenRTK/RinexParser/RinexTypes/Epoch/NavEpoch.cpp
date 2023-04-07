#include "NavEpoch.hpp"

NavEpoch::NavEpoch() : Epoch()
{

}

NavEpoch::NavEpoch(int year, int month, int day, int hour, int minute, double second) : Epoch(year, month, day, hour, minute, second)
{
	
}

std::unique_ptr<Satellite> NavEpoch::TryAddSatellite(const Satellite& sv)
{
    std::unique_ptr<Satellite> currentSatellite;

    auto it = std::find_if(this->_Satellites.begin(), this->_Satellites.end(), [&](const Satellite& sat)
        {
            return sat == sv;
        });

    if (it == this->_Satellites.end())
    {
        this->_Satellites.push_back(sv);
        currentSatellite = std::make_unique<Satellite>(this->_Satellites.back());
    }
    else
    {
        currentSatellite = std::make_unique<Satellite>(*it);
    }

    return currentSatellite;
}

NavEpoch::~NavEpoch()
{
	_Satellites.clear();
}

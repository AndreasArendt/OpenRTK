#pragma once

#include "Epoch.hpp"
#include "../../RinexTypes/Satellite.hpp"
#include "../../NavData/NavData.hpp"
#include "../../NavData/GpsNavData.hpp"

#include <vector>
#include <memory>

class NavEpoch : public Epoch
{
private:	
	std::vector<std::unique_ptr<NavData>> _NavigationData;

public:
	// getters	
	std::vector<std::unique_ptr<NavData>> const& NavigationData() const { return this->_NavigationData; }
	
	//setters
	std::vector<std::unique_ptr<NavData>>& NavigationData() { return this->_NavigationData; }

	// ctor & dtor
	NavEpoch(int year, int month, int day, int hour, int minute, double second);
	~NavEpoch();	

    // Custom copy constructor
    NavEpoch(const NavEpoch& other) : Epoch(other)
    {        
        for (const auto& navData : other._NavigationData) {
            _NavigationData.emplace_back(navData->clone());
        }
    }

    // Custom copy assignment operator
    NavEpoch& operator=(const NavEpoch& other)
    {
        if (this != &other) {
            Epoch::operator=(other);            
            _NavigationData.clear();
            for (const auto& navData : other._NavigationData) {
                _NavigationData.emplace_back(navData->clone());
            }
        }
        return *this;
    }
};


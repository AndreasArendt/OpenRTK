#pragma once

#include "Epoch.hpp"
#include "../../RinexTypes/Satellite.hpp"

#include <vector>
#include <memory>

class NavEpoch : public Epoch
{
private:	
	std::vector<Satellite> _Satellites;

public:
	// getters	
	std::vector<Satellite> const& Satellites() const { return this->_Satellites; }
		
	// public functions
	void addSatellite(const Satellite& sv);

	// ctor & dtor
	NavEpoch();
	NavEpoch(int year, int month, int day, int hour, int minute, double second);
	~NavEpoch();

	// Custom copy constructor
	NavEpoch(const NavEpoch& other) : Epoch(other)
	{
		for (const auto& sat : other._Satellites) {
			_Satellites.emplace_back(sat);
		}
	}

	// Custom copy assignment operator
	NavEpoch& operator=(const NavEpoch& other)
	{
		if (this != &other) {
			Epoch::operator=(other);
			_Satellites.clear();
			for (const auto& sat : other._Satellites) {
				_Satellites.emplace_back(sat);
			}
		}
		return *this;
	}
};


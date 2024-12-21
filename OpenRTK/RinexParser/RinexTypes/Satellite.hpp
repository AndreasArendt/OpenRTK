#pragma once

#include "../../Utils/AbstractSatellite.hpp"
#include "../../PrecisePositioning/Ephemeris/Ephemeris.hpp"
#include "../NavData/NavData.hpp"
#include "../ObsData/ObsData.hpp"

#include <format>
#include <string>
#include <vector>
#include <memory>

class Satellite : public AbstractSatellite
{
private:
	// members    
	std::vector<std::unique_ptr<NavData>> _NavigationData;
	std::vector<ObsData> _ObservationData;
	std::map<ObservationBand, std::vector<std::unique_ptr<Ephemeris>>> _Ephemeris;
	NavData* findClosestTime(double targetTime);

	//private functions
	double CalcSatelliteTxTime(double time, CodeObservation& cObs);
	void addEphemeris(ObservationBand band, std::unique_ptr<Ephemeris> ephemeris);

public:
	// getters        
	std::vector<std::unique_ptr<NavData>> const& NavigationData() const { return this->_NavigationData; }
	std::vector<ObsData> const& ObservationData() const { return this->_ObservationData; }
	std::map<ObservationBand, std::vector<std::unique_ptr<Ephemeris>>> const& SatelliteEphemeris() const { return this->_Ephemeris; }

	// public functions    
	void addNavData(std::unique_ptr<NavData> navdata);
	void addObsData(ObsData obsdata);
	void calcEphemeris();

	// ctor & dtor
	Satellite() = default;
	Satellite(SvSystem svSystem, int svNumber) : AbstractSatellite(svSystem, svNumber) {}
	Satellite(std::string satStr) : AbstractSatellite(satStr) {}
	Satellite(const Satellite& other) : AbstractSatellite(other)
	{
		// Create new unique pointers and copy the underlying objects
		for (const auto& ptr : other._NavigationData)
		{
			this->_NavigationData.push_back(ptr->clone());
		}

		for (const auto& ptr : other._ObservationData)
		{
			this->_ObservationData.push_back(ptr);
		}

		for (const auto& pair : other._Ephemeris)
		{
			for (const auto& ephemerisPtr : pair.second)
			{
				this->addEphemeris(pair.first, ephemerisPtr->clone());
			}
		}
	}

	~Satellite()
	{
		this->_NavigationData.clear();
		this->_ObservationData.clear();
		this->_Ephemeris.clear();
	}

	// operator overloading
	Satellite& operator=(Satellite& other) noexcept;
};
#include "Satellite.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris/GalileoEphemeris.hpp"

#include <iostream>
#include <iomanip>

Satellite::Satellite() : _SvSystem(SvSystem::UNKNOWN), _SvNumber(-1)
{
}

Satellite::Satellite(SvSystem svSystem, int svNumber) : _SvSystem(svSystem), _SvNumber(svNumber)
{
}

Satellite::Satellite(std::string satStr)
{
	this->_SvSystem = static_cast<SvSystem>(satStr.at(0));
	this->_SvNumber = parseInt(satStr.substr(1, 2));
}

Satellite::Satellite(const Satellite& other) :
	_SvSystem(other._SvSystem),
	_SvNumber(other._SvNumber)
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

	for (const auto& ptr : other._Ephemeris) 
	{
		this->_Ephemeris.push_back(ptr->clone());
	}
	
}

Satellite::~Satellite()
{	
	this->_NavigationData.clear();
	this->_ObservationData.clear();
	this->_Ephemeris.clear();
}

void Satellite::addNavData(std::unique_ptr<NavData> navdata)
{
	this->_NavigationData.push_back(std::move(navdata));
}

void Satellite::addObsData(ObsData obsdata)
{
	this->_ObservationData.push_back(obsdata);
}

void Satellite::calcEphimeris()
{
	switch (this->_SvSystem)
	{
	case SvSystem::GALILEO:
		for (auto& nav : this->_NavigationData)
		{
			auto eph = std::make_unique<GalileoEphemeris>();
			
			auto duration_since_epoch = nav.get()->EpochTime().time_since_epoch();
			double utc = std::chrono::duration<double>(duration_since_epoch).count();
						
			eph->CalcEphemeris(*nav.get());
			
			std::cout << std::fixed << std::setprecision(10) << "E" << this->SvNumber() << "," << utc << "," << eph->Position_E().x() << "," << eph->Position_E().y() << "," << eph->Position_E().z() << std::endl;

			this->_Ephemeris.push_back(std::move(eph));
		}
		break;
	default:
		break;
	}
}

Satellite& Satellite::operator=(Satellite& other) noexcept
{
	if (this != &other)
	{
		this->_SvSystem = other._SvSystem;
		this->_SvNumber = other._SvNumber;
		
		// Copy the vector of NavigationData
		this->_NavigationData.clear(); // Clear current vector contents
		for (const auto& navData : other._NavigationData)
		{
			this->_NavigationData.push_back(navData->clone());
		}

		this->_Ephemeris.clear(); // Clear current vector contents
		for (const auto& eph : other._Ephemeris)
		{
			this->_Ephemeris.push_back(eph->clone());
		}
	}
	return *this;
}

bool Satellite::operator==(const Satellite& other) const
{
	return (_SvSystem == other.SVSystem()) && (_SvNumber == other.SvNumber());
}

bool Satellite::operator!=(const Satellite& other) const
{
	return (_SvSystem != other.SVSystem()) || (_SvNumber != other.SvNumber());
}
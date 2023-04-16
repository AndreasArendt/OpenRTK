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
	for (const auto& ptr : other._NavigationData) {
		this->_NavigationData.push_back(ptr->clone());
	}
}

Satellite::~Satellite()
{
	this->_NavigationData.clear();
}

void Satellite::addNavData(std::unique_ptr<NavData> navdata)
{
	_NavigationData.push_back(std::move(navdata));
}

void Satellite::calcEphimeris()
{
	switch (this->_SvSystem)
	{
	case SvSystem::GALILEO:
		for (auto& nav : this->_NavigationData)
		{
			auto eph = GalileoEphemeris();
			eph.CalcEphemeris((*nav.get()));
			this->_Ephemeris.emplace_back(eph);
		}

		/*this->_Ephemeris.CalcGalileoEphimeris( this->_NavigationData);
				
		for (auto ephemeris : this->_Ephemeris.Position_E())
		{
			std::cout << std::fixed << std::setprecision(10) << "E" << this->SvNumber() << "," << ephemeris.x() << "," << ephemeris.y() << "," << ephemeris.z() << std::endl;
		}*/

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
		this->_Ephemeris = other._Ephemeris;
		// Copy the vector of NavigationData
		this->_NavigationData.clear(); // Clear current vector contents
		for (const auto& navData : other._NavigationData)
		{
			this->_NavigationData.push_back(navData->clone());
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
#include "AbstractSatellite.hpp"

#include "astring.hpp"

AbstractSatellite::AbstractSatellite() : _SvSystem(SvSystem::UNKNOWN), _SvNumber(-1)
{
}

AbstractSatellite::AbstractSatellite(SvSystem svSystem, int svNumber) : _SvSystem(svSystem), _SvNumber(svNumber)
{
}

AbstractSatellite::AbstractSatellite(std::string satStr)
{
	this->_SvSystem = static_cast<SvSystem>(satStr.at(0));
	this->_SvNumber = util::astring::parseInt(satStr.substr(1, 2));
}

AbstractSatellite::AbstractSatellite(const AbstractSatellite& other) : _SvSystem(other._SvSystem), _SvNumber(other._SvNumber)
{	
}

AbstractSatellite& AbstractSatellite::operator=(AbstractSatellite& other) noexcept
{
	if (this != &other)
	{
		this->_SvSystem = other._SvSystem;
		this->_SvNumber = other._SvNumber;
	}

	return *this;
}

bool AbstractSatellite::operator==(const AbstractSatellite& other) const
{
	return (this->_SvSystem == other.SVSystem()) && (this->_SvNumber == other.SvNumber());
}

bool AbstractSatellite::operator!=(const AbstractSatellite& other) const
{
	return (this->_SvSystem != other.SVSystem()) || (this->_SvNumber != other.SvNumber());
}
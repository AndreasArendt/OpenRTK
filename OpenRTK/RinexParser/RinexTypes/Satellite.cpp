#include "Satellite.hpp"

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

Satellite::~Satellite()
{
	//_Epochs.clear();
}

Satellite& Satellite::operator=(const Satellite& other)
{
	if (this != &other)
	{
		_SvSystem = other.SVSystem();
		_SvNumber = other.SvNumber();
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
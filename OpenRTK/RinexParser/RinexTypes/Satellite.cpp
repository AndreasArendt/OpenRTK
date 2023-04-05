#include "Satellite.hpp"
#include "../../Utils/astring.hpp"

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

/*template <typename... Args, std::enable_if_t<std::is_constructible_v<NavEpoch, Args...>, int>>
void Satellite::addNavEpoch(Args&&... args)
{
	this->_NavEpochs.emplace_back(std::make_unique<NavEpoch>(std::forward<Args>(args)...));
}*/

Satellite::~Satellite()
{
	//this->_NavEpochs.clear();
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
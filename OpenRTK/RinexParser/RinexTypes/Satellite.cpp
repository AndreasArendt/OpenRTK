#include "Satellite.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris/GalileoEphemeris.hpp"
#include "../../Transformations/Transformation.hpp"

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

double Satellite::CalcSatelliteTxTime(double time, CodeObservation& cObs)
{
	return time - cObs.Pseudorange__m() / Transformation::SpeedOfLight__mDs;
}

NavData* Satellite::findClosestTime(double targetTime)
{
	double minDifference = std::numeric_limits<double>::max();
	NavData* closestData = nullptr;

	for (const auto& data : this->_NavigationData)
	{
		double difference = std::abs(data->ReceiverTime() - targetTime);
		if ((difference < minDifference) && ( std::abs(difference) < 14400.0)) //GAL #define MAXDTOE_GAL 14400.0             /* max time difference to Galileo Toe (s) */
		{
			minDifference = difference;
			closestData = data.get();
		}
	}

	return closestData;
}

void Satellite::calcEphemeris()
{
	switch (this->_SvSystem)
	{
	case SvSystem::GALILEO:
		for (auto& obs : this->_ObservationData)
		{
			auto eph = std::make_unique<GalileoEphemeris>();

			double time = obs.ReceiverTime();

			NavData* nav = this->findClosestTime(time);

			if (nav != nullptr)
			{
				auto galNav = dynamic_cast<GalileoNavData*>(nav);

				double delta_t = nav->ReceiverTime() - time;

				// TODO: do for all frequencies!!
				
				// transmission time correction
				double transmission_time__s = obs.CodeObservations().begin()->second.Pseudorange__m() / Transformation::SpeedOfLight__mDs;
				time = time - transmission_time__s;

				//// apply clock correction - taken from RTKLIB eph2clk
				//double t = time - nav->ReceiverTime();
				//double ts = t;

				//for (int i = 0; i < 2; i++) 
				//{					
				//	t = ts - (galNav->SV_ClockBias__s() + galNav->SV_ClockDrift__sDs() * t + galNav->SV_ClockDriftRate__sDs2() * t * t);
				//}

				//auto dt = galNav->SV_ClockBias__s() + galNav->SV_ClockDrift__sDs() * t + galNav->SV_ClockDriftRate__sDs2() * t * t;
				//time = time - dt;

				eph->CalcEphemeris(*nav, time);
				this->_Ephemeris.push_back(std::move(eph));
			}
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
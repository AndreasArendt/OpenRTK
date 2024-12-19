#include "Satellite.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris/GalileoEphemeris.hpp"
#include "../../PrecisePositioning/Ephemeris/GpsEphemeris.hpp"
#include "../NavData/Galileo/GalileoNavData.hpp"
#include "../NavData/Gps/GpsNavData.hpp"
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

Satellite::Satellite(const Satellite& other) : _SvSystem(other._SvSystem), _SvNumber(other._SvNumber)
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
			this->InsertEphemeris(pair.first, ephemerisPtr->clone());
		}
	}
}

Satellite::~Satellite()
{
	this->_NavigationData.clear();
	this->_ObservationData.clear();
	this->_Ephemeris.clear();
}

void Satellite::InsertEphemeris(ObservationBand band, std::unique_ptr<Ephemeris> ephemeris)
{		
	auto it = this->_Ephemeris.find(band);
	if (it != this->_Ephemeris.end())
	{
		auto& EphemerisVector = it->second;
		EphemerisVector.push_back(std::move(ephemeris));
	}
	else
	{
		// If the key doesn't exist, create a new vector and move the unique_ptr into it
		std::vector<std::unique_ptr<Ephemeris>> newEphemerisVector;
		newEphemerisVector.push_back(std::move(ephemeris));
		this->_Ephemeris[band] = std::move(newEphemerisVector);
	}
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

	for (auto& navdata : this->_NavigationData)
	{
		double difference = std::abs(navdata->Epoche().PosixEpochTime__s() - targetTime);
		if ((difference < minDifference) && ( std::abs(difference) < 14400.0)) //GAL #define MAXDTOE_GAL 14400.0             /* max time difference to Galileo Toe (s) */
		{
			minDifference = difference;
			closestData = navdata.get();
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
			double time = obs.Epoche().PosixEpochTime__s();

			NavData* nav = this->findClosestTime(time);
		
			if (nav != nullptr)
			{
				auto galNav = dynamic_cast<GalileoNavData*>(nav);				
				
				for (const auto& [band, code] : obs.CodeObservations()) 
				{					
					auto svHealth = GalileoSvHealth::fromBitfield(galNav->SvHealth());
					auto eph = std::make_unique<GalileoEphemeris>(svHealth);
					
					// transmission time correction
					double transmission_time__s = code.Pseudorange__m() / Transformation::SpeedOfLight__mDs;
					time = time - transmission_time__s;
					time = time - eph->CalcClockOffset(*nav, time);

					eph->CalcEphemeris(*nav, time, obs.Epoche().PosixEpochTime__s());					
					this->InsertEphemeris(band, std::move(eph));
				}
			}
		}			
		break;
	case SvSystem::GPS:
		for (auto& obs : this->_ObservationData)
		{
			double time = obs.Epoche().PosixEpochTime__s();

			NavData* nav = this->findClosestTime(time);

			if (nav != nullptr)
			{
				auto gpsNav = dynamic_cast<GpsNavData*>(nav);

				for (const auto& [band, code] : obs.CodeObservations())
				{
					auto svHealth = GpsSvHealth::fromBitfield(gpsNav->SvHealth());
					auto eph = std::make_unique<GpsEphemeris>(svHealth);

					// transmission time correction
					double transmission_time__s = code.Pseudorange__m() / Transformation::SpeedOfLight__mDs;
					time = time - transmission_time__s;
					time = time - eph->CalcClockOffset(*nav, time);

					eph->CalcEphemeris(*nav, time, obs.Epoche().PosixEpochTime__s());
					this->InsertEphemeris(band, std::move(eph));
				}
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
	
		for (const auto& pair : other._Ephemeris)
		{
			for (const auto& ephemerisPtr : pair.second)
			{
				this->InsertEphemeris(pair.first, ephemerisPtr->clone());
			}
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
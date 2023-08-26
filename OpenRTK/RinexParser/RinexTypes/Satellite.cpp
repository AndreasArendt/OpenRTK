#include "Satellite.hpp"
#include "../../Utils/astring.hpp"
#include "../../PrecisePositioning/Ephemeris/GalileoEphemeris.hpp"
#include "../../Transformations/Transformation.hpp"

#include "../../RinexParser/NavData/rtktypes.h"

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

	for (auto& data : this->_NavigationData)
	{
		NavData* pnavData = data.get();
		GalileoNavData* galNav = dynamic_cast<GalileoNavData*>(pnavData);		

		double difference = std::abs(galNav->ToeEpoch() - targetTime);		
		if ((difference < minDifference) && ( std::abs(difference) < 14400.0)) //GAL #define MAXDTOE_GAL 14400.0             /* max time difference to Galileo Toe (s) */
		{
			minDifference = difference;
			closestData = data.get();
		}
	}

	return closestData;
}

double adjweek(double t, double t0)
{
	double tt = difftime(t, t0);
	if (tt < -302400.0) 
		return t + 604800.0;
	if (tt > 302400.0) 
		return t -604800.0;
	return t;
}

double epoch2time(const double* ep)
{
	const int doy[] = { 1,32,60,91,121,152,182,213,244,274,305,335 };
	double time = 0;
	int days, year = (int)ep[0], mon = (int)ep[1], day = (int)ep[2];
	double sec = ep[5];

	if (year < 1970 || 2099 < year || mon < 1 || 12 < mon) return time;

	/* leap year if year%4==0 in 1901-2099 */
	days = (year - 1970) * 365 + (year - 1969) / 4 + doy[mon - 1] + day - 2 + (year % 4 == 0 && mon >= 3 ? 1 : 0);
	//sec = (int)floor(ep[5]);	
	time = (time_t)days * 86400 + (int)ep[3] * 3600 + (int)ep[4] * 60 + sec;	
	return time;
}

static const double gpst0[] = { 1980,1, 6,0,0,0 }; /* gps time reference */
double gpst2time(int week, double sec)
{
	double t = epoch2time(gpst0);

	if (sec < -1E9 || 1E9 < sec) 
		sec = 0.0;
	t += (time_t)86400 * 7 * week + (int)sec;	
	return t;
}

gtime_t rtk_timeadd(gtime_t t, double sec)
{
	double tt;

	t.sec += sec; tt = floor(t.sec); t.time += (int)tt; t.sec -= tt;
	return t;
}

double rtk_timediff(gtime_t t1, gtime_t t2)
{
	return difftime(t1.time, t2.time) + t1.sec - t2.sec;
}

gtime_t rtk_adjweek(gtime_t t, gtime_t t0)
{
	double tt = rtk_timediff(t, t0);
	if (tt < -302400.0) return rtk_timeadd(t, 604800.0);
	if (tt > 302400.0) return rtk_timeadd(t, -604800.0);
	return t;
}

gtime_t rtk_gpst2time(int week, double sec)
{
	gtime_t t = { .time = 315964800, .sec = 0.0 };

	if (sec < -1E9 || 1E9 < sec) sec = 0.0;
	t.time += (time_t)86400 * 7 * week + (int)sec;
	t.sec = sec - (int)sec;
	return t;
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

				double delta_t = nav->Toc__s() - time;

				// TODO: do for all frequencies!!
				
				// transmission time correction
				double transmission_time__s = obs.CodeObservations().begin()->second.Pseudorange__m() / Transformation::SpeedOfLight__mDs;
				time = time - transmission_time__s;
				
				eph->CalcEphemeris(*nav, time);
				//this->_Ephemeris.push_back(std::move(eph));

								
				eph_t seph;		
				gtime_t toc = { .time = (int)galNav->Toc__s(), .sec = galNav->Toc__s() - (int)galNav->Toc__s() };
				gtime_t toe = rtk_adjweek(rtk_gpst2time(galNav->GalWeek(), galNav->Toe__s()), toc);

				seph.toe = galNav->ToeEpoch();
				seph.toc = galNav->Toc__s();
				seph.A = galNav->SqrtA___sqrtm() * galNav->SqrtA___sqrtm();
				seph.e = galNav->Eccentricity();
				seph.i0 = galNav->i0__rad();
				seph.OMG0 = galNav->Omega0__rad();
				seph.omg = galNav->Omega__rad();
				seph.M0 = galNav->M0__rad();
				seph.deln = galNav->DeltaN__radDs();
				seph.OMGd = galNav->Omega0__rad();
				seph.idot = galNav->Idot__radDs();
				seph.crc = galNav->Crc__m();
				seph.crs = galNav->Crs__m();
				seph.cuc = galNav->Cuc__rad();
				seph.cus = galNav->Cus__rad();
				seph.cic = galNav->Cic__rad();
				seph.cis = galNav->Cis__rad();
				seph.toes = galNav->Toe__s();        /* Toe (s) in week */
				seph.f0 = galNav->SV_ClockBias__s();
				seph.f1 = galNav->SV_ClockDrift__sDs();
				seph.f2 = galNav->SV_ClockDriftRate__sDs2();
			

				double rs[3];
				double dts;
				double var;

				//eph->eph2pos(gtime_t time, const eph_t * eph, double* rs, double* dts, double* var)
				eph->eph2pos(time, &seph, rs, &dts, &var);
				//eph->_Position_E = ECEF_Position(rs[0], rs[1], rs[2]);

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
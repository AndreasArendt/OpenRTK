#include "Postprocessor.hpp"

#include <vector>


Postprocessor::Postprocessor()
{
	
}

Postprocessor::~Postprocessor()
{

}

bool Postprocessor::CheckSatelliteHealthy(Satellite& sv)
{	
	return true;
}

void Postprocessor::ShuffleData(std::vector<Satellite> satellites)
{
	for (Satellite& sv : satellites)
	{
		for (ObsData obs : sv.ObservationData())
		{
			obs.Epoche().PosixEpochTime__s();
		}
	}
}

//Timestamp, vector<Ephemeris>, vector<Observations>
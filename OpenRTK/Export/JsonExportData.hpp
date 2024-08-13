#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../Transformations/ECEF_Position.hpp"
#include "../RinexParser/RinexTypes/Epoch/Epoch.hpp"

class jEpoch : public Epoch
{
public:
	nlohmann::json to_json()
	{
		return {
			{"year", this->Year()},
			{"month", this->Month()},
			{"day", this->Day()},
			{"hour", this->Hour()},
			{"minute", this->Minute()},
			{"second", this->Second()}			
		};
	}
};

class GenericObservation
{
public:
	double Band_1;
	double Band_2;
	double Band_5;

	nlohmann::json to_json()
	{
		return
		{
			{"Band_1", this->Band_1},
			{"Band_2", this->Band_2},
			{"Band_5", this->Band_5}
		};
	}

	GenericObservation() : Band_1(0), Band_2(0), Band_5(0) { }
};

class SatelliteObservation
{
public:
	std::string SatelliteSystem;
	ECEF_Position ECEF_Position;
	double ClockOffset;
	double ClockDrift;
	double RelativisticError;

	bool IsHealthy;

	GenericObservation Code;
	GenericObservation Carrier;
	GenericObservation Doppler;
	GenericObservation Snr;

	nlohmann::json to_json()
	{
		return {
		   {"SatelliteSystem", this->SatelliteSystem},
		   {"ECEF_Position", {{"x", this->ECEF_Position.x()}, {"y", this->ECEF_Position.y()}, {"z", this->ECEF_Position.z()}}},
		   {"ClockOffset", this->ClockOffset},
		   {"ClockDrift", this->ClockDrift},
		   {"RelativisticError", this->RelativisticError},
           {"IsHealthy", this->IsHealthy},
		   {"Code", this->Code.to_json()},
		   {"Carrier", this->Carrier.to_json()},
		   {"Doppler", this->Doppler.to_json()},
		   {"Snr", this->Snr.to_json()}
		};
	}
};

class SatelliteData
{
public:
	jEpoch Epoch;
	std::vector<SatelliteObservation> Observations;		
};


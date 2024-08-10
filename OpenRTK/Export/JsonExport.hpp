#pragma once

#include <vector>
#include <string>

#include "JsonExportData.hpp"
#include "../RinexParser/RinexTypes/Satellite.hpp"
#include "../PrecisePositioning/Ephemeris/Ephemeris.hpp"

class JsonExport
{
private:
	std::vector<SatelliteData> _SatelliteData;

	SatelliteObservation CreateSatObservation(Satellite sv, ObsData obs, const Ephemeris& ephemeris);
	double GetCodeObservationIfExist(ObsData obs, ObservationBand band);
	double GetPhaseObservationIfExist(ObsData obs, ObservationBand band);
	double GetDopplerObservationIfExist(ObsData obs, ObservationBand band);
	double GetSnrObservationIfExist(ObsData obs, ObservationBand band);

public:	
	void Export(std::vector<Satellite>& satellites, std::string path);

	// ctor & dtor
	JsonExport() {};
	~JsonExport() { this->_SatelliteData.clear(); };
};


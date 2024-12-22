#pragma once

#include <vector>
#include <filesystem>

#include "JsonExportData.hpp"
#include "../RinexParser/RinexTypes/Satellite.hpp"
#include "../PrecisePositioning/Ephemeris/Ephemeris.hpp"
#include "../SP3/SP3Satellite.hpp"

class JsonExport
{
private:
	std::vector<SatelliteData> _RinexSatelliteData;
	std::vector<PreciseEphemerisData> _PreciseEphemerisData;

	SatelliteObservation CreateSatObservation(const Satellite& sv, const ObsData& obs, const Ephemeris& ephemeris);

	double GetCodeObservationIfExist(ObsData obs, ObservationBand band);
	double GetPhaseObservationIfExist(ObsData obs, ObservationBand band);
	double GetDopplerObservationIfExist(ObsData obs, ObservationBand band);
	double GetSnrObservationIfExist(ObsData obs, ObservationBand band);

	void CollectRinexData(std::vector<Satellite>& satellites);
	void CollectPreciseEphemerisData(const std::vector<SP3Satellite>& satellites);

public:	
	void ExportObservations(std::vector<Satellite>& satellites, std::filesystem::path path);	
	void ExportPreciseEphemeris(const std::vector<SP3Satellite>& satellites, std::filesystem::path path);

	// ctor & dtor
	JsonExport() {};
	~JsonExport()
	{ 
		this->_RinexSatelliteData.clear(); 
		this->_PreciseEphemerisData.clear();
	}	
};


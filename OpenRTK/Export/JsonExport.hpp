#pragma once

#include <vector>
#include <filesystem>

#include "JsonExportData.hpp"
#include "../RinexParser/RinexTypes/Satellite.hpp"
#include "../PrecisePositioning/Ephemeris/Ephemeris.hpp"
#include "../SP3/SP3Satellite.hpp"
#include "../CLK/ClkSatellite.hpp"

class JsonExport
{
private:
	std::vector<SatelliteData> _RinexSatelliteData;
	std::vector<PreciseEphemerisData> _PreciseEphemerisData;
	std::vector<PreciseClockData> _PreciseClockData;

	SatelliteObservation CreateSatObservation(const Satellite& sv, const ObsData& obs, const Ephemeris& ephemeris);

	double GetCodeObservationIfExist(ObsData obs, ObservationBand band);
	double GetPhaseObservationIfExist(ObsData obs, ObservationBand band);
	double GetDopplerObservationIfExist(ObsData obs, ObservationBand band);
	double GetSnrObservationIfExist(ObsData obs, ObservationBand band);

	void CollectRinexData(std::vector<Satellite>& satellites);
	void CollectPreciseEphemerisData(const std::vector<SP3Satellite>& satellites);

	void CollectPreciseClockData(const std::vector<ClkSatellite>& clocks);

public:	
	void ExportCombined(std::vector<Satellite>& satellites, std::filesystem::path path);	
	void ExportObs(const std::vector<Satellite>& const satellites, std::filesystem::path path);
	void ExportNav(const std::vector<Satellite>& const satellites, std::filesystem::path path);
	void ExportPreciseEphemeris(const std::vector<SP3Satellite>& satellites, std::filesystem::path path);
	void ExportPreciseClock(const std::vector<ClkSatellite>& clocks, std::filesystem::path path);

	// ctor & dtor
	JsonExport();
	~JsonExport();
};


#include "ExportHelper.hpp"

#include "JsonExport.hpp"
#include <filesystem>

void ExportHelper::exportSP3(const std::vector<SP3Satellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	std::filesystem::path jsonFile = "";
	
	// Export Precise Ephemeris data if available	
	jsonFile = std::filesystem::path(this->_outdir) / "precise_satdata.json";
	JsonExport().ExportPreciseEphemeris(satellites, jsonFile);
}

void ExportHelper::exportPreciseClock(const std::vector<ClkSatellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	// Export Precise Clock data if available	
	std::filesystem::path jsonFile = "";

	jsonFile = std::filesystem::path(this->_outdir) / "precise_clkdata.json";
	JsonExport().ExportPreciseClock(satellites, jsonFile);
}

void ExportHelper::exportCombinedObsNav(const std::vector<Satellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	std::filesystem::path jsonFile = "";
		
	// Export Rinex observations if available	
	jsonFile = std::filesystem::path(this->_outdir) / "satdata.json";

	std::vector<Satellite> Satellites = satellites; //tmp fix
	JsonExport().ExportCombined(Satellites, jsonFile);
}

void ExportHelper::exportNav(const std::vector<Satellite>& const satellites)
{	
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	std::filesystem::path jsonFile = "";

	auto jExport = JsonExport();

	jsonFile = std::filesystem::path(this->_outdir) / "navdata.json";	
	JsonExport().ExportNav(satellites, jsonFile);
}

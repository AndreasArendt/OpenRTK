#include "ExportHelper.hpp"

#include "JsonExport.hpp"
#include <filesystem>

void ExportHelper::exportSP3(const std::vector<SP3Satellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	std::filesystem::path jsonFile = "";
	auto jExport = JsonExport();

	// Export Precise Ephemeris data if available	
	jsonFile = std::filesystem::path(this->_outdir) / "precise_satdata.json";
	jExport.ExportPreciseEphemeris(satellites, jsonFile);
}

void ExportHelper::exportPreciseClock(const std::vector<ClkSatellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	// Export Precise Clock data if available
	auto jExport = JsonExport();
	std::filesystem::path jsonFile = "";

	jsonFile = std::filesystem::path(this->_outdir) / "precise_clkdata.json";
	jExport.ExportPreciseClock(satellites, jsonFile);
}

void ExportHelper::exportObservations(const std::vector<Satellite>& const satellites)
{
	// create if not existing
	std::filesystem::create_directory(this->_outdir);

	std::filesystem::path jsonFile = "";

	auto jExport = JsonExport();

	// Export Rinex observations if available	
	jsonFile = std::filesystem::path(this->_outdir) / "satdata.json";

	std::vector<Satellite> Satellites;
	std::copy_if(satellites.begin(), satellites.end(), std::back_inserter(Satellites), [](const auto& sv)
		{
			return (sv.SVSystem() == SvSystem::GALILEO) || (sv.SVSystem() == SvSystem::GPS);
		});

	for (Satellite& sv : Satellites)
	{
		sv.calcEphemeris();
	}

	jExport.ExportObservations(Satellites, jsonFile);	
}

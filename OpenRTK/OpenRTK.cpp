// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "PrecisePositioning/PPP.hpp"
#include "Export/CsvExport.hpp"

#include <vector>
#include <memory>

using namespace std;

int main()
{
	auto rnxParser = RinexParser();

	//std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/AUBG00DEU_R_20231231000_01H_30S_MO.obs";
	//std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20231231000_01H_MN.nav";

	//std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/AUBG00DEU_R_20232220000_01D_30S_MO.obs";
	//std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20232220000_01D_MN.nav";
	
	std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/ORID00MKD_R_20230090000_01D_30S_MO.obs";
	std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/ORID00MKD_R_20230090000_01D_MN.nav";

	//std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/HARL00NLD_R_20233100000_01D_30S_MO.obs";
	//std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/HARL00NLD_R_20233100000_01D_MN.nav";
	
	rnxParser.Parse(obsPath);
	rnxParser.Parse(navPath);

	auto spp = SPP();

	std::vector<Satellite> Satellites;
	std::copy_if(rnxParser.Satellites().begin(), rnxParser.Satellites().end(), std::back_inserter(Satellites), [](const auto& sv)
		{
			return (sv.SVSystem() == SvSystem::GALILEO) || (sv.SVSystem() == SvSystem::GPS);
		});

	for (Satellite& sv : Satellites)
	{
		sv.calcEphemeris();				
	}

	// Single Point Processing
	spp.Calculate(Satellites);

	CsvExport::ExportEphemeris(Satellites, "D:/Projekte/OpenRTK/Analysis/Ephemeris_exp.csv");
	CsvExport::ExportObsData(Satellites, "D:/Projekte/OpenRTK/Analysis/Observation_exp.csv");

	return 0;
}

// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"

#include "PrecisePositioning/SPP.hpp"
#include "Export/JsonExport.hpp"

#include <vector>
#include <memory>

using namespace std;


int main()
{	
	auto rnxParser = RinexParser();
	
	//std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/ORID00MKD_R_20230090000_01D_30S_MO.obs";
	//std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/ORID00MKD_R_20230090000_01D_MN.nav";
	auto sp3Parser = SP3Parser();
	std::string sp3Path = "D:/Projekte/OpenRTK/data/ESA0MGNFIN_20242000000_01D_05M_ORB.SP3";
		
	sp3Parser.Parse(sp3Path);
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

	auto jExport = JsonExport();
	jExport.Export(Satellites, "D:/Projekte/OpenRTK/data/satdata.json");

	return 0;
}

// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"
#include "CLK/ClkParser.hpp"

#include "PrecisePositioning/SPP.hpp"
#include "Export/JsonExport.hpp"

#include <vector>
#include <memory>

int main()
{	
	auto rnxParser = RinexParser();
	auto sp3Parser = SP3Parser();
	auto clkParser = ClkParser();

	auto jExport = JsonExport();

	std::string sp3Path = "D:/Projekte/OpenRTK/data/ESA0MGNFIN_20242000000_01D_05M_ORB.SP3";
	std::string clkPath = "D:/Projekte/OpenRTK/data/ESA0MGNFIN_20242000000_01D_30S_CLK.CLK";

	clkParser.Parse(clkPath);
	jExport.ExportPreciseClock(clkParser.Satellites(), "D:/Projekte/OpenRTK/data/precise_clkdata.json");

	sp3Parser.Parse(sp3Path);		
	jExport.ExportPreciseEphemeris(sp3Parser.Satellites(), "D:/Projekte/OpenRTK/data/precise_satdata.json");

	rnxParser.Parse(obsPath);
	rnxParser.Parse(navPath);

	//auto spp = SPP();

	std::vector<Satellite> Satellites;
	std::copy_if(rnxParser.Satellites().begin(), rnxParser.Satellites().end(), std::back_inserter(Satellites), [](const auto& sv)
		{
			return (sv.SVSystem() == SvSystem::GALILEO) || (sv.SVSystem() == SvSystem::GPS);
		});

	for (Satellite& sv : Satellites)
	{
		sv.calcEphemeris();				
	}

	//// Single Point Processing
	//spp.Calculate(Satellites);

	//auto jExport = JsonExport();
	jExport.ExportObservations(Satellites, "D:/Projekte/OpenRTK/data/satdata.json");

	return 0;
}

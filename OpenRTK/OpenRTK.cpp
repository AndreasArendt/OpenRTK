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

	std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/AUBG00DEU_R_20231231000_01H_30S_MO.obs";
	std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20231260000_01D_MN.nav";	
	
	rnxParser.Parse(obsPath);
	rnxParser.Parse(navPath);

	auto ppp = PPP();

	std::vector<Satellite> GalileoSatellites;
	std::copy_if(rnxParser.Satellites().begin(), rnxParser.Satellites().end(), std::back_inserter(GalileoSatellites), [](const auto& sv)
		{
			return sv.SVSystem() == SvSystem::GALILEO;
		});

	for (Satellite& sv : GalileoSatellites)
	{
		sv.calcEphemeris();

		//ppp.CalcUserPosition(sv.Ephemeris(), sv.
	}

	CsvExport().ExportEphemeris(GalileoSatellites, "D:/Projekte/OpenRTK/Analysis/Ephemeris_exp.csv");

	return 0;
}

// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "PrecisePositioning/PPP.hpp"

#include <memory>

using namespace std;

int main()
{
	auto rnxParser = RinexParser();
		
	std::string obsPath = "D:/Projekte/OpenRTK/data/sampledata/obs/OBE400DEU_R_20230690000_01D_30S_MO.obs";
	std::string navPath = "D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20230690000_01D_MN.nav";

	rnxParser.Parse(obsPath);
	rnxParser.Parse(navPath);

	//auto firstEp = obsParser->Epochs().front();
	//double utc = firstEp.ConvertEpochTimeToUTC();

	//std::unique_ptr<RinexNavParser> navParser = std::make_unique<RinexNavParser>();
	//navParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20230690000_01D_MN.nav");
	//
	//auto ppp = PPP();

	//for (Satellite sv : navParser->Satellites())
	//{
	//	sv.calcEphimeris();
	//	//ppp.CalcUserPosition(sv.Ephemeris(), sv.
	//}

	return 0;
}

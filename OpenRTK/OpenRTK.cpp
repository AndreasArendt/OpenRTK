// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexObsParser/RinexObsParser.hpp"
#include "RinexParser/RinexNavParser/RinexNavParser.hpp"

using namespace std;

int main()
{
	std::unique_ptr<RinexObsParser> obsParser = std::make_unique<RinexObsParser>();
	
	//obsParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/obs/OBE400DEU_R_20221550000_01D_30S_MO.obs");
	//obsParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/obs/OBE400DEU_R_20230690000_01D_30S_MO.obs");
	
	//auto firstEp = obsParser->Epochs().front();
	//double utc = firstEp.ConvertEpochTimeToUTC();

	std::unique_ptr<RinexNavParser> navParser = std::make_unique<RinexNavParser>();
	navParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20230690000_01D_MN.nav");

	return 0;
}

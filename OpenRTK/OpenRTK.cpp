// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexObsParser/RinexObsParser.hpp"
#include "RinexNavParser/RinexNavParser.hpp"

using namespace std;

int main()
{
	//std::unique_ptr<RinexObsParser> obsParser = std::make_unique<RinexObsParser>();
	
	//obsParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/obs/OBE400DEU_R_20221550000_01D_30S_MO.obs");
	//rParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/obs/OBE400DEU_R_20230690000_01D_30S_MO.obs");	
	
	//RinexEpoch x = obsParser->Epochs().front();
	//double utc = x.ConvertEpochTimeToUTC();

	std::unique_ptr<RinexNavParser> navParser = std::make_unique<RinexNavParser>();
	navParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/nav/AUBG00DEU_R_20230690000_01D_MN.nav");

	return 0;
}

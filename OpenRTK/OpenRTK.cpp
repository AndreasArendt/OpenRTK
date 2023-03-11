// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexReader/RinexObservationParser.hpp"

using namespace std;

int main()
{
	std::unique_ptr<RinexObservationParser> obsParser = std::make_unique<RinexObservationParser>();
	
	obsParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/OBE400DEU_R_20221550000_01D_30S_MO.obs");
	//rParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/OBE400DEU_R_20230690000_01D_30S_MO.obs");	
	
	return 0;
}

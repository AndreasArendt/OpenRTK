// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexReader/RinexObservationParser.hpp"

using namespace std;

int main()
{
	std::unique_ptr<RinexObservationParser> rParser = std::make_unique<RinexObservationParser>();
	
	rParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/OBE400DEU_R_20221550000_01D_30S_MO/OBE400DEU_R_20221550000_01D_30S_MO.obs");
	
	return 0;
}

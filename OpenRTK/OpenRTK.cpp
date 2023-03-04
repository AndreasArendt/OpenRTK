// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexReader/RinexParser.hpp"

using namespace std;

int main()
{
	std::unique_ptr<RinexParser> rParser = std::make_unique<RinexParser>();
	
	rParser->ParseFile("D:/Projekte/OpenRTK/data/sampledata/OBE400DEU_R_20221550000_01D_30S_MO/OBE400DEU_R_20221550000_01D_30S_MO.obs");
	return 0;
}

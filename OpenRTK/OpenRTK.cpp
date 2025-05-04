// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "InputParserHelper.hpp"

int main(int argc, char* argv[])
{
	InputParserHelper inputParser = InputParserHelper();	
	inputParser.parseArguments(argc, argv);
	inputParser.runApplication();
}
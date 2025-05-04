#pragma once

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"
#include "CLK/ClkParser.hpp"

#include "Export/ExportHelper.hpp"
#include "InputParser/InputParser.hpp"

class InputParserHelper
{
private:
	RinexParser rnxParser;
	ClkParser clkParser;
	SP3Parser sp3Parser;
	InputParser inputParser;

	ExportHelper exportHelper;

	void registerParameters();

	void parseExportObs(std::optional<std::vector<std::string>> args);
	void parseExportNav(std::optional<std::vector<std::string>> args);
	void parseExportClk(std::optional<std::vector<std::string>> args);
	void parseExportSp3(std::optional<std::vector<std::string>> args);
	void setOutDir(std::optional<std::vector<std::string>> args);

public:
	InputParserHelper();
	void parseArguments(int argc, char* argv[]);
};


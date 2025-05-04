#pragma once

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"
#include "CLK/ClkParser.hpp"

#include "Export/ExportHelper.hpp"
#include "InputParser/InputParser.hpp"

#include <filesystem>

class InputParserHelper
{
private:
	RinexParser _rnxParser;
	ClkParser _clkParser;
	SP3Parser _sp3Parser;
	InputParser _inputParser;

	ExportHelper _exportHelper;

	// settings
	bool _hasObs = false;
	bool _hasNav = false;
	bool _hasClk = false;
	bool _hasSp3 = false;

	std::string _outputDir = std::filesystem::current_path().string();

	bool _exportCombined = false;
	bool _exportObs = false;
	bool _exportNav = false;
	bool _exportClk = false;
	bool _exportSp3 = false;

	// Input Parser Function
	void registerArguments();

	void parseObs(std::optional<std::vector<std::string>> args);
	void parseNav(std::optional<std::vector<std::string>> args);
	void parseClk(std::optional<std::vector<std::string>> args);
	void parseSp3(std::optional<std::vector<std::string>> args);
	void setOutDir(std::optional<std::vector<std::string>> args);
	void setOutputFileType(std::optional<std::vector<std::string>> args);

public:
	InputParserHelper();
	void parseArguments(int argc, char* argv[]);
	void runApplication();
};


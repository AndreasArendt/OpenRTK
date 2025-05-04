#include "InputParserHelper.hpp"

#include <filesystem>

// Todo - fix this
bool _hasObs = false;
bool _hasNav = false;

InputParserHelper::InputParserHelper()
{
	// Register parameters
	this->registerParameters();

	// Set default output directory
	exportHelper = ExportHelper(std::filesystem::current_path().string());
}

void InputParserHelper::parseExportObs(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->rnxParser.Parse(args.value().front());
		_hasObs = true;
		if (_hasNav && _hasObs)
		{
			this->exportHelper.exportObservations(this->rnxParser.Satellites());
		}
	}
}

void InputParserHelper::parseExportNav(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->rnxParser.Parse(args.value().front());
		_hasNav = true;
		if (_hasNav && _hasObs)
		{
			this->exportHelper.exportObservations(this->rnxParser.Satellites());
		}
	}
}

void InputParserHelper::parseExportClk(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->clkParser.Parse(args.value().front());
		this->exportHelper.exportPreciseClock(this->clkParser.Satellites());
	}
}

void InputParserHelper::parseExportSp3(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->sp3Parser.Parse(args.value().front());
		this->exportHelper.exportSP3(this->sp3Parser.Satellites());
	}
}

void InputParserHelper::setOutDir(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->exportHelper.setOutDir(args.value().front());
	}
}

void InputParserHelper::registerParameters()
{
	inputParser.addParameter("obs", "Parse Rinex obs data [.obs]", [this](std::optional<std::vector<std::string>> args) { parseExportObs(args); });
	inputParser.addParameter("nav", "Parse Rinex nav data [.nav]", [this](std::optional<std::vector<std::string>> args) { parseExportNav(args);	});
	inputParser.addParameter("clk", "Parse Precise Clock data [.clk]", [this](std::optional<std::vector<std::string>> args) { parseExportClk(args);	});
	inputParser.addParameter("sp3", "Parse Precise Satellite Ephemerides data [.sp3]", [this](std::optional<std::vector<std::string>> args) { parseExportSp3(args); });
	inputParser.addParameter("outdir", "Output data directory", [this](std::optional<std::vector<std::string>> args) { setOutDir(args);	}, 0);
}

void InputParserHelper::parseArguments(int argc, char* argv[])
{
	std::string combinedArgs;
	for (int i = 1; i < argc; ++i)
	{
		combinedArgs += std::string(argv[i]) + " ";
	}
	inputParser.parse(combinedArgs);
}

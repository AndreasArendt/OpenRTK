#include "InputParserHelper.hpp"
#include "Utils/path.hpp"

#include <filesystem>
#include <numeric>

InputParserHelper::InputParserHelper()
{
	// Register parameters
	this->registerArguments();
	this->_exportHelper = ExportHelper();
}

void InputParserHelper::parseDir(std::optional<std::vector<std::string>> args)
{
	if (!args || args->empty())
		return;

	this->_outputDir = args.value().front();

	try
	{
		for (const auto& entry : fs::directory_iterator(args.value().front()))
		{
			if (fs::is_regular_file(entry.status()))
			{
				std::vector<std::string> file = { entry.path().string() };

				// SP3
				if (hasExtension(entry.path(), ".SP3"))
				{
					this->parseSp3(file);
				}

				// OBS
				if (hasExtension(entry.path(), ".obs"))
				{					
					this->parseObs(file);
				}

				// OBS
				if (hasExtension(entry.path(), ".nav"))
				{
					this->parseNav(file);
				}

				// CLK
				if (hasExtension(entry.path(), ".CLK"))
				{
					this->parseNav(file);
				}
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << '\n';
	}
}

void InputParserHelper::parseObs(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->_rnxParser.Parse(args.value().front());
		this->_hasObs = true;
	}
}

void InputParserHelper::parseNav(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->_rnxParser.Parse(args.value().front());
		this->_hasNav = true;
	}
}

void InputParserHelper::parseClk(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->_clkParser.Parse(args.value().front());
		this->_hasClk = true;
	}
}

void InputParserHelper::parseSp3(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->_sp3Parser.Parse(args.value().front());
		this->_hasSp3 = true;
	}
}

void InputParserHelper::setOutDir(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		this->_outputDir = args.value().front();
	}
}

void InputParserHelper::setOutputFileType(std::optional<std::vector<std::string>> args)
{
	if (args && !args->empty())
	{
		std::string arguments = std::accumulate(args.value().begin(), args.value().end(), std::string(""));

		// export combined file
		if (arguments.find("c") != std::string::npos)
		{
			this->_exportCombined = true;
		}

		// export nav only file
		if (arguments.find("n") != std::string::npos)
		{
			this->_exportNav = true;
		}

		// export obs only file
		if (arguments.find("o") != std::string::npos)
		{
			this->_exportObs = true;
		}
	}
}

void InputParserHelper::registerArguments()
{
	this->_inputParser.addParameter("dir", "Parse Rinex data in directory, also sets output directory [--outdir]", [this](std::optional<std::vector<std::string>> args) { parseDir(args); });
	this->_inputParser.addParameter("obs", "Parse Rinex obs data [.obs]", [this](std::optional<std::vector<std::string>> args) { parseObs(args); });
	this->_inputParser.addParameter("nav", "Parse Rinex nav data [.nav]", [this](std::optional<std::vector<std::string>> args) { parseNav(args);	});
	this->_inputParser.addParameter("clk", "Parse Precise Clock data [.clk]", [this](std::optional<std::vector<std::string>> args) { parseClk(args);	});
	this->_inputParser.addParameter("sp3", "Parse Precise Satellite Ephemerides data [.sp3]", [this](std::optional<std::vector<std::string>> args) { parseSp3(args); });
	this->_inputParser.addParameter("export", "Export File Type [c (combined); n (nav); o (obs)]", [this](std::optional<std::vector<std::string>> args) { setOutputFileType(args);	});
	this->_inputParser.addParameter("outdir", "Output data directory", [this](std::optional<std::vector<std::string>> args) { setOutDir(args);	}, 0);
}

void InputParserHelper::parseArguments(int argc, char* argv[])
{
	std::string combinedArgs;
	for (int i = 1; i < argc; ++i)
	{
		combinedArgs += std::string(argv[i]) + " ";
	}

	this->_inputParser.parse(combinedArgs);
}

void InputParserHelper::runApplication()
{
	// initialize exportHelper
	this->_exportHelper.setOutDir(this->_outputDir);

	// Combined Export
	if (this->_hasObs && this->_hasNav && this->_exportCombined)
	{
		std::cout << "Exporting combined obs and nav data";
		this->_exportHelper.exportCombinedObsNav(this->_rnxParser.Satellites());
		std::cout << " - done " << std::endl;
	}

	// Obs Export
	if (this->_hasObs && this->_exportObs)
	{
		std::cout << "Exporting obs data";
		this->_exportHelper.exportObs(this->_rnxParser.Satellites());
		std::cout << " - done " << std::endl;
	}

	// Nav Export
	if (this->_hasNav && this->_exportNav)
	{
		std::cout << "Exporting nav data";
		this->_exportHelper.exportNav(this->_rnxParser.Satellites());
		std::cout << " - done " << std::endl;
	}

	// Sp3 Export
	if (this->_hasSp3)
	{
		this->_exportHelper.exportSP3(this->_sp3Parser.Satellites());

	}

	// Clk Export
	if (this->_hasClk)
	{
		this->_exportHelper.exportPreciseClock(this->_clkParser.Satellites());
	}
}

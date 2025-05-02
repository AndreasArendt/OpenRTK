// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"
#include "CLK/ClkParser.hpp"

#include "PrecisePositioning/SPP.hpp"
#include "Export/ExportHelper.hpp"
#include "InputParser/InputParser.hpp"

#include <filesystem>
#include <vector>
#include <memory>
#include <iostream>

RinexParser rnxParser;
ClkParser clkParser;
SP3Parser sp3Parser;
InputParser inputParser;

ExportHelper exportHelper;

// Todo - fix this
bool _hasObs = false;
bool _hasNav = false;

int main(int argc, char* argv[])
{
	exportHelper = ExportHelper(std::filesystem::current_path().string());

	inputParser.addParameter("obs", "Parse Rinex obs data [.obs]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				rnxParser.Parse(args.value().front());
				_hasObs = true;
				if (_hasNav && _hasObs)
				{
					exportHelper.exportObservations(rnxParser.Satellites());
				}
			}
		});
	inputParser.addParameter("nav", "Parse Rinex nav data [.nav]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				rnxParser.Parse(args.value().front());
				_hasNav = true;
				if (_hasNav && _hasObs)
				{
					exportHelper.exportObservations(rnxParser.Satellites());
				}
			}
		});
	inputParser.addParameter("clk", "Parse Precise Clock data [.clk]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				clkParser.Parse(args.value().front());
				exportHelper.exportPreciseClock(clkParser.Satellites());
			}
		});
	inputParser.addParameter("sp3", "Parse Precise Satellite Ephemerides data [.sp3]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				sp3Parser.Parse(args.value().front());
				exportHelper.exportSP3(sp3Parser.Satellites());
			}
		});

	inputParser.addParameter("outdir", "Output data directory", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				exportHelper.setOutDir(args.value().front());				
			}
		}, 0);


	std::string combinedArgs;
	for (int i = 1; i < argc; ++i) 
	{
		combinedArgs += std::string(argv[i]) + " ";
	}
	inputParser.parse(combinedArgs);
}
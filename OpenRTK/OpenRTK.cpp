// OpenRTK.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "RinexParser/RinexParser.hpp"
#include "SP3/SP3Parser.hpp"
#include "CLK/ClkParser.hpp"

#include "PrecisePositioning/SPP.hpp"
#include "Export/JsonExport.hpp"
#include "InputParser/InputParser.hpp"

#include <vector>
#include <memory>
#include <iostream>
#include <filesystem>

RinexParser rnxParser;
ClkParser clkParser;
SP3Parser sp3Parser;
InputParser inputParser;

void exportData(const std::vector<std::string> outdir)
{
	// create if not existing
	std::filesystem::create_directory(outdir.front());
	std::filesystem::path jsonFile = "";

	auto jExport = JsonExport();

	// Export Rinex observations if available
	if (inputParser.hasParameter("obs") && inputParser.hasParameter("nav"))
	{
		jsonFile = std::filesystem::path(outdir.front()) / "satdata.json";

		std::vector<Satellite> Satellites;
		std::copy_if(rnxParser.Satellites().begin(), rnxParser.Satellites().end(), std::back_inserter(Satellites), [](const auto& sv)
			{
				return (sv.SVSystem() == SvSystem::GALILEO) || (sv.SVSystem() == SvSystem::GPS);
			});

		for (Satellite& sv : Satellites)
		{
			sv.calcEphemeris();
		}

		jExport.ExportObservations(Satellites, jsonFile);
	}

	// Export Precise Clock data if available
	if (inputParser.hasParameter("clk"))
	{
		jsonFile = std::filesystem::path(outdir.front()) / "precise_clkdata.json";
		jExport.ExportPreciseClock(clkParser.Satellites(), jsonFile);
	}

	// Export Precise Ephemeris data if available
	if (inputParser.hasParameter("sp3"))
	{
		jsonFile = std::filesystem::path(outdir.front()) / "precise_satdata.json";
		jExport.ExportPreciseEphemeris(sp3Parser.Satellites(), jsonFile);
	}
}

int main(int argc, char* argv[])
{
	inputParser.addParameter("obs", "Parse Rinex obs data [.obs]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				rnxParser.Parse(args.value().front());
			}
		});
	inputParser.addParameter("nav", "Parse Rinex nav data [.nav]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				rnxParser.Parse(args.value().front());
			}
		});
	inputParser.addParameter("clk", "Parse Precise Clock data [.clk]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				clkParser.Parse(args.value().front());
			}
		});
	inputParser.addParameter("sp3", "Parse Precise Satellite Ephemerides data [.sp3]", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				sp3Parser.Parse(args.value().front());
			}
		});

	inputParser.addParameter("outdir", "Output data directory", [](std::optional<std::vector<std::string>> args)
		{
			if (args && !args->empty())
			{
				exportData(args.value());
			}
		});


	std::string combinedArgs;
	for (int i = 1; i < argc; ++i) 
	{
		combinedArgs += std::string(argv[i]) + " ";
	}
	inputParser.parse(combinedArgs);
}
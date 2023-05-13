#pragma once

#include "../RinexParser/RinexTypes/Satellite.hpp"
#include "../RinexParser/ObsData/ObsData.hpp"

#include <vector>
#include <string>
#include <fstream>

class CsvExport
{
private:
	static std::ofstream OpenFile(std::string path);

	CsvExport();
	~CsvExport();

public:
	static void ExportEphemeris(const std::vector<Satellite>& satellites, std::string path);
	static void ExportObsData(const std::vector<Satellite>& satellites, std::string path);
};
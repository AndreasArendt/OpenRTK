#pragma once

#include "../RinexParser/RinexTypes/Satellite.hpp"

#include <vector>
#include <string>

class CsvExport
{
private:

public:
	static void ExportEphemeris(const std::vector<Satellite>& satellites, std::string path);

	CsvExport();
	~CsvExport();

};
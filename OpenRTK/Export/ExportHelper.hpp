#pragma once

#include "../RinexParser/RinexTypes/Satellite.hpp"
#include "../CLK/ClkSatellite.hpp"
#include "../SP3/SP3Satellite.hpp"

#include <string>
#include <vector>

class ExportHelper
{
private:
	std::string _outdir;

public:
	void exportCombinedObsNav(const std::vector<Satellite>& const satellites);
	void exportNav(const std::vector<Satellite>& const satellites);
	void exportPreciseClock(const std::vector<ClkSatellite>& const satellites);
	void exportSP3(const std::vector<SP3Satellite>& const satellites);

	ExportHelper() = default;
	ExportHelper(const std::string& outdir) : _outdir(outdir) {}
	~ExportHelper() = default;
		
	const std::string const getOutDir() const { return this->_outdir; }
	void setOutDir(const std::string& outdir) { this->_outdir = outdir; }
};


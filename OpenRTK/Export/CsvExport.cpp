#include "CsvExport.hpp"

#include <iostream>
#include <iomanip>
#include <memory>

CsvExport::CsvExport()
{
}

CsvExport::~CsvExport()
{
}
std::ofstream CsvExport::OpenFile(std::string path)
{
	std::ofstream fid;
	fid.open(path, std::ofstream::trunc);
	return fid;
}

void CsvExport::ExportObsData(const std::vector<Satellite>& satellites, std::string path)
{
	auto fid = CsvExport::OpenFile(path);
	fid << "SvSystem,UTC,Code_1,Code_2,Code_5" << std::endl;

	for (Satellite const& sv : satellites)
	{
		for (auto const& obs : sv.ObservationData())
		{
			fid << std::fixed << std::setprecision(10) << "E" << sv.SvNumber() << "," << obs.Utc();

			for (auto band : { ObservationBand::Band_1, ObservationBand::Band_2, ObservationBand::Band_5 })
			{
				if (obs.CodeObservations().contains(band))
				{
					double code_1 = obs.CodeObservations().at(band).Pseudorange__m();
					fid << "," << code_1;
				}
				else
				{
					fid << "," << 0;
				}
			}
			fid << std::endl;
		}
	}

	fid.close();
}

void CsvExport::ExportEphemeris(const std::vector<Satellite>& satellites, std::string path)
{
	auto fid = CsvExport::OpenFile(path);
	fid << "SvSystem,UTC, x,y,z" << std::endl;

	for (Satellite const& sv : satellites)
	{
		for (auto const& eph : sv.Ephemeris())
		{
			fid << std::fixed << std::setprecision(10) << "E" << sv.SvNumber() << "," << eph->Utc() << "," << eph->Position_E().x() << "," << eph->Position_E().y() << "," << eph->Position_E().z() << std::endl;
		}
	}

	fid.close();
}
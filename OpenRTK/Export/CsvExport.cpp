#include "CsvExport.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>

CsvExport::CsvExport()
{
}

CsvExport::~CsvExport()
{
}

void CsvExport::ExportEphemeris(const std::vector<Satellite>& satellites, std::string path)
{
	std::ofstream fid;
	fid.open(path, std::ofstream::trunc);
	
	fid << "SvSystem,UTC, x,y,z" << std::endl;

	for (Satellite sv : satellites)
	{		
		for (auto const& eph: sv.Ephemeris())
		{			
			fid << std::fixed << std::setprecision(10) << "E" << sv.SvNumber() << "," << eph->Utc() << ","  << eph->Position_E().x() << "," << eph->Position_E().y() << "," << eph->Position_E().z() << std::endl;
		}
	}

	fid.close();
}
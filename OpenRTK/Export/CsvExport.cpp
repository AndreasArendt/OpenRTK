#include "CsvExport.hpp"

#include <iostream>
#include <iomanip>
#include <memory>

#include "../PrecisePositioning/Ephemeris/GalileoEphemeris.hpp"

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
	fid << "SvSystem,UTC,Code_1,Code_2,Code_5,Phase_1,Phase_2,Phase_5,Snr_1,Snr_2,Snr_5" << std::endl;

	for (Satellite const& sv : satellites)
	{
		for (auto const& obs : sv.ObservationData())
		{
			fid << std::fixed << std::setprecision(10) << "E" << sv.SvNumber() << "," << obs.Epoche().Toc__s();

			// Code
			for (auto band : { ObservationBand::Band_1, ObservationBand::Band_2, ObservationBand::Band_5 })
			{
				if (obs.CodeObservations().contains(band))
				{
					double code = obs.CodeObservations().at(band).Pseudorange__m();
					fid << "," << code;
				}
				else
				{
					fid << "," << 0;
				}
			}

			// Phase
			for (auto band : { ObservationBand::Band_1, ObservationBand::Band_2, ObservationBand::Band_5 })
			{
				if (obs.PhaseObservations().contains(band))
				{
					double cycles = obs.PhaseObservations().at(band).Carrierphase__Cycles();
					fid << "," << cycles;
				}
				else
				{
					fid << "," << 0;
				}
			}

			// SNR
			for (auto band : { ObservationBand::Band_1, ObservationBand::Band_2, ObservationBand::Band_5 })
			{
				if (obs.SnrObservations().contains(band))
				{
					double snr = obs.SnrObservations().at(band).SNR();
					fid << "," << snr;
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
	fid << "SvSystem,UTC,Toe,ObsToc,x,y,z,SvClockOffset,RelativisticError,Health" << std::endl;

	for (Satellite const& sv : satellites)
	{
		for (auto& eph : sv.Ephemeris())
		{
			auto gEph = dynamic_cast<GalileoEphemeris*>(eph.get());

			fid << std::fixed << std::setprecision(10) << "E" << sv.SvNumber() << "," 
													   << eph->Utc() << "," 
													   << gEph->Toe__s() << ","
													   << eph->Obstime__s() << ","
													   << eph->Position_E().x() << "," 
													   << eph->Position_E().y() << "," 
													   << eph->Position_E().z() << "," 
													   << eph->SatelliteClockError__s() << ","
													   << eph->RelativisticError__s() << ","
													   << eph->SvHealth().Health()
													   << std::endl;
		}
	}

	fid.close();
}
#include "JsonExport.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

SatelliteObservation JsonExport::CreateSatObservation(Satellite sv, ObsData obs, const Ephemeris* ephemeris)
{
	SatelliteObservation satObs;
	satObs.SatelliteSystem = sv.SvString();
		
	satObs.ECEF_Position = ephemeris->Position_E();
	satObs.ClockOffset = ephemeris->SatelliteClockError__s();
	satObs.ClockDrift = ephemeris->SatelliteClockDrift__1Ds();
	satObs.RelativisticError = ephemeris->RelativisticError__s();

	satObs.Code = GenericObservation();
	satObs.Code.Band_1 = obs.CodeObservations().at(ObservationBand::Band_1).Pseudorange__m();
	satObs.Code.Band_2 = obs.CodeObservations().at(ObservationBand::Band_2).Pseudorange__m();
	satObs.Code.Band_5 = obs.CodeObservations().at(ObservationBand::Band_5).Pseudorange__m();

	satObs.Carrier = GenericObservation();
	satObs.Carrier.Band_1 = obs.PhaseObservations().at(ObservationBand::Band_1).Carrierphase__Cycles();
	satObs.Carrier.Band_2 = obs.PhaseObservations().at(ObservationBand::Band_2).Carrierphase__Cycles();
	satObs.Carrier.Band_5 = obs.PhaseObservations().at(ObservationBand::Band_5).Carrierphase__Cycles();

	satObs.Doppler = GenericObservation();
	satObs.Doppler.Band_1 = obs.DopplerObservations().at(ObservationBand::Band_1).Doppler__Hz();
	satObs.Doppler.Band_2 = obs.DopplerObservations().at(ObservationBand::Band_2).Doppler__Hz();
	satObs.Doppler.Band_5 = obs.DopplerObservations().at(ObservationBand::Band_5).Doppler__Hz();

	satObs.Snr = GenericObservation();
	satObs.Snr.Band_1 = obs.SnrObservations().at(ObservationBand::Band_1).SNR();
	satObs.Snr.Band_2 = obs.SnrObservations().at(ObservationBand::Band_2).SNR();
	satObs.Snr.Band_5 = obs.SnrObservations().at(ObservationBand::Band_5).SNR();

	return satObs;
}

void JsonExport::Export(std::vector<Satellite>& satellites, std::string path)
{
	// iterate over all satellites
	for (const auto& sv : satellites)
	{
		// iterate over all observations of current satellite
		for (int i = 0; i < sv.ObservationData().size(); i++)
		{
			auto obs = sv.ObservationData().at(i);			
			const auto& ephemeris = sv.SatelliteEphemeris().at(ObservationBand::Band_1).at(i).get();
			SatelliteObservation satObs = CreateSatObservation(sv, obs, ephemeris);

			// if data empty, insert - else iterate 
			if (this->_SatelliteData.empty())
			{
				auto satData = SatelliteData();				
				satData.Epoch = obs.Epoche();
				satData.Observations.push_back(satObs);

				this->_SatelliteData.push_back(satData);
			}
			else
			{
				// check if any entry for TOC already exists
				for(int j = 0; j < this->_SatelliteData.size(); j++)				
				{
					auto& sdat = this->_SatelliteData.at(j);

					// found current epoch
					if (sdat.Epoch == obs.Epoche())
					{						
						sdat.Observations.push_back(satObs);												
						break;
					}
					// insert in sorted order
					else if (sdat.Epoch < obs.Epoche())
					{			
						auto satData = SatelliteData();
						satData.Epoch = obs.Epoche();
						satData.Observations.push_back(satObs);
												
						this->_SatelliteData.insert(this->_SatelliteData.begin() + j, satData);
						break;
					}
				}
			}
		}		
	}
}
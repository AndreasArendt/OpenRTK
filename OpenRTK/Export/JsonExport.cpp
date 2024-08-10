#include "JsonExport.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

double JsonExport::GetCodeObservationIfExist(ObsData obs, ObservationBand band)
{
	if (obs.CodeObservations().find(band) != obs.CodeObservations().end())
	{
		return obs.CodeObservations().at(band).Pseudorange__m();
	}	

	return 0;
}

double JsonExport::GetPhaseObservationIfExist(ObsData obs, ObservationBand band)
{
	if (obs.PhaseObservations().find(band) != obs.PhaseObservations().end())
	{
		return obs.PhaseObservations().at(band).Carrierphase__Cycles();
	}

	return 0;
}

double JsonExport::GetDopplerObservationIfExist(ObsData obs, ObservationBand band)
{
	if (obs.DopplerObservations().find(band) != obs.DopplerObservations().end())
	{
		return obs.DopplerObservations().at(band).Doppler__Hz();
	}

	return 0;
}

double JsonExport::GetSnrObservationIfExist(ObsData obs, ObservationBand band)
{
	if (obs.SnrObservations().find(band) != obs.SnrObservations().end())
	{
		return obs.SnrObservations().at(band).SNR();
	}

	return 0;
}


SatelliteObservation JsonExport::CreateSatObservation(const Satellite& sv, const ObsData& obs, const Ephemeris& ephemeris)
{
	SatelliteObservation satObs;
	satObs.SatelliteSystem = sv.SvString();
		
	satObs.ECEF_Position = ephemeris.Position_E();
	satObs.ClockOffset = ephemeris.SatelliteClockError__s();
	satObs.ClockDrift = ephemeris.SatelliteClockDrift__1Ds();
	satObs.RelativisticError = ephemeris.RelativisticError__s();

	satObs.Code = GenericObservation();
	satObs.Code.Band_1 = this->GetCodeObservationIfExist(obs, ObservationBand::Band_1);
	satObs.Code.Band_2 = this->GetCodeObservationIfExist(obs, ObservationBand::Band_2);
	satObs.Code.Band_5 = this->GetCodeObservationIfExist(obs, ObservationBand::Band_5);

	satObs.Carrier = GenericObservation();
	satObs.Carrier.Band_1 = this->GetPhaseObservationIfExist(obs, ObservationBand::Band_1);
	satObs.Carrier.Band_2 = this->GetPhaseObservationIfExist(obs, ObservationBand::Band_2);
	satObs.Carrier.Band_5 = this->GetPhaseObservationIfExist(obs, ObservationBand::Band_5);

	satObs.Doppler = GenericObservation();
	satObs.Doppler.Band_1 = this->GetDopplerObservationIfExist(obs, ObservationBand::Band_1);
	satObs.Doppler.Band_2 = this->GetDopplerObservationIfExist(obs, ObservationBand::Band_2);
	satObs.Doppler.Band_5 = this->GetDopplerObservationIfExist(obs, ObservationBand::Band_5);

	satObs.Snr = GenericObservation();
	satObs.Snr.Band_1 = this->GetSnrObservationIfExist(obs, ObservationBand::Band_1);
	satObs.Snr.Band_2 = this->GetSnrObservationIfExist(obs, ObservationBand::Band_2);
	satObs.Snr.Band_5 = this->GetSnrObservationIfExist(obs, ObservationBand::Band_5);

	return satObs;
}

void JsonExport::CollectData(std::vector<Satellite>& satellites)
{
	// iterate over all satellites
	for (const auto& sv : satellites)
	{
		// iterate over all observations of current satellite
		for (int i = 0; i < sv.ObservationData().size(); i++)
		{
			auto obs = sv.ObservationData().at(i);			
			const auto& ephemeris = *sv.SatelliteEphemeris().at(ObservationBand::Band_1).at(i).get();
			SatelliteObservation satObs = this->CreateSatObservation(sv, obs, ephemeris);

			// prepare data to be inserted
			auto satData = SatelliteData();				
			satData.Epoch = static_cast<jEpoch>(obs.Epoche());
			satData.Observations.push_back(satObs);

			// if data empty, insert - else iterate 
			if (this->_SatelliteData.empty())
			{
				this->_SatelliteData.push_back(satData);
			}
			else
			{
				// check if any entry for TOC already exists
				for(int j = 0; j < this->_SatelliteData.size(); j++)				
				{
					auto& sdat = this->_SatelliteData.at(j);
					
					// found current epoch
					if (obs.Epoche() == sdat.Epoch)
					{				
						this->_SatelliteData.at(j).Observations.push_back(satObs);						
						break;
					}
					// insert in sorted order					
					else if (obs.Epoche() > sdat.Epoch)
					{
						if (this->_SatelliteData.size() > j + 1)
						{
							auto& next = this->_SatelliteData.at(j+1);
							if (obs.Epoche() > sdat.Epoch && obs.Epoche() < next.Epoch)
							{
								this->_SatelliteData.insert(this->_SatelliteData.begin() + j + 1, satData);
								break;
							}
						}
						else
						{
							this->_SatelliteData.insert(this->_SatelliteData.begin() + j +1, satData);						
							break;
						}																			
					}
				}
			}
		}		
	}
}

void JsonExport::Export(std::vector<Satellite>& satellites, std::filesystem::path path)
{
	this->CollectData(satellites);

	nlohmann::json j;

	for (const auto& sat : this->_SatelliteData) 
	{
		nlohmann::json satJson;
		satJson["Epoch"] = static_cast<jEpoch>(sat.Epoch).to_json();

		nlohmann::json observationsJson = nlohmann::json::array();
		for (const auto& obs : sat.Observations) 
		{
			observationsJson.push_back(static_cast<SatelliteObservation>(obs).to_json());
		}

		satJson["Observations"] = observationsJson;
		j["SatelliteData"].push_back(satJson);
	}

	std::ofstream file(path);
	file << j;
}
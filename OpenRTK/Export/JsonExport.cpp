#include "JsonExport.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

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

	satObs.ECEF_Position	 = ephemeris.Position_E();
	satObs.ClockOffset		 = ephemeris.SatelliteClockError__s();
	satObs.ClockDrift		 = ephemeris.SatelliteClockDrift__1Ds();
	satObs.RelativisticError = ephemeris.RelativisticError__s();
	satObs.IsHealthy		 = ephemeris.SatelliteHealth().Health() == 0; // should work for now!

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
	auto epochDataMap = std::unordered_map<Epoch, SatelliteData>();

	// iterate over all satellites
	for (const auto& sv : satellites)
	{
		// iterate over all observations of current satellite
		for (int i = 0; i < sv.ObservationData().size(); i++)
		{
			auto obs = sv.ObservationData().at(i);	

			// check if Ephemeris data available
			if (!sv.SatelliteEphemeris().contains(ObservationBand::Band_1))
			{
				break;
			}

			const auto& ephemeris = *sv.SatelliteEphemeris().at(ObservationBand::Band_1).at(i).get();
			SatelliteObservation satObs = this->CreateSatObservation(sv, obs, ephemeris);

			// prepare data to be inserted
			auto satData = SatelliteData();				
			satData.PosixEpochTime__s = obs.Epoche().PosixEpochTime__s();
			satData.Observations.push_back(satObs);

			if (epochDataMap.contains(obs.Epoche()))
			{
				epochDataMap.at(obs.Epoche()).Observations.push_back(satObs);
			}
			else
			{
				epochDataMap.insert(std::pair<Epoch, SatelliteData>(obs.Epoche(), satData));
			}
		}		
	}

	this->_SatelliteData.clear();
	this->_SatelliteData.reserve(epochDataMap.size());
	for (const auto& [epoch, data] : epochDataMap)
	{
		this->_SatelliteData.push_back(data);
	}

	// sorting
	std::sort(this->_SatelliteData.begin(), this->_SatelliteData.end(),
		[](const SatelliteData& a, const SatelliteData& b) {
			return a.PosixEpochTime__s < b.PosixEpochTime__s;
		});
}

void JsonExport::Export(std::vector<Satellite>& satellites, std::filesystem::path path)
{
	this->CollectData(satellites);

	json j;

	j["SatelliteData"] = json::array();

	for (auto& sv : this->_SatelliteData)
	{
		json satJson;
		satJson["PosixEpochTime__s"] = sv.PosixEpochTime__s;
				
		json observationsJson = json::array();
		
		for (auto& obs : sv.Observations)
		{
			observationsJson.push_back(obs.to_json());
		}

		satJson["Observations"] = std::move(observationsJson);
		j["SatelliteData"].push_back(std::move(satJson));
	}

	std::ofstream file(path);
	file << j;
}
//
//  RinexParser.cpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#include "RinexObsParser.hpp"
#include "../RinexTypes/ObservationType.hpp"
#include "../RinexTypes/ObservationBand.hpp"
#include "../RinexTypes/ObservationAttribute.hpp"
#include "../RinexTypes/Satellite.hpp"
#include "../Observations/CodeObservation.hpp"
#include "../Observations/DopplerObservation.hpp"
#include "../Observations/PhaseObservation.hpp"
#include "../Observations/SignalStrengthObservation.hpp"
#include "../../Utils/strim.hpp"
#include "../../Utils/astring.hpp"

#include <utility>

#define RINEX_VERSION_DEFINITION         "RINEX VERSION / TYPE"
#define RINEX_APPROX_POSITION_DEFINITION "APPROX POSITION XYZ"
#define RINEX_ANTENNA_DELTA_DEFINITION   "ANTENNA: DELTA H/E/N"
#define RINEX_OBS_TYPE_DEFINITION        "SYS / # / OBS TYPES"

RinexObsParser::RinexObsParser()
{    
}

RinexObsParser::~RinexObsParser()
{            
    this->_Epochs.clear();
    this->_ObservationDefinitions.clear();    
    this->_AntennaOffset.reset();
    this->_ApproximateMarkerPosition.reset();
}

void RinexObsParser::ReadEpochHeader(std::string line) {
    // Check if input line has the expected length
    if (line.length() < 33)
    {
        std::cerr << "Error: Invalid epoch header line: " << line << std::endl;
        return;
    }

    try
    {
        // Parse substrings to integers/double
        int year = parseInt(line.substr(2, 4));
        int month = parseInt(line.substr(7, 2));
        int day = parseInt(line.substr(10, 2));
        int hour = parseInt(line.substr(13, 2));
        int minute = parseInt(line.substr(16, 2));
        double second = parseDouble(line.substr(19, 10));
        int epochFlag = parseInt(line.substr(31, 1));
        int numberSVs = parseInt(line.substr(33, 2));

        // Create new Epoch object and add it to the _Epochs vector
        _Epochs.emplace_back(year, month, day, hour, minute, second, epochFlag, numberSVs);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Invalid epoch header line: " << line << std::endl;
    }
}

void RinexObsParser::ReadEpochObservation(std::string line)
{       
    auto satellite = std::make_unique<Satellite>(Satellite(line.substr(0, 3)));
    const auto& SvObsDefinitions = _ObservationDefinitions.at(satellite->SVSystem());
    
    // See 6.7 in rinex standard how observations are formatted    
    unsigned int StartIndex = 3; //offset of first observation
    for (const auto& obsDef : SvObsDefinitions)
    {         
        // less data avaibale in observation as in specified Header
        if (line.length() < StartIndex + 14)
        {
            continue;
        }

        std::string data = line.substr(StartIndex, 14);

        // no valid data
        if (data.find_first_not_of(' ') == std::string::npos)
        {
            StartIndex += 16;
            continue;
        }

        switch (obsDef.GetObservationType())
        {            
            case ObservationType::Code: //Pseudorange
            {   
                double psuedorange = parseDouble(data);                    
                CodeObservation cObs = CodeObservation(obsDef.GetObservationBand(), std::move(satellite), psuedorange);
                _Epochs.back().AddCodeObservation(cObs);                
                break;  
            }
            case ObservationType::Phase: //Carrierphase
            {
                double phase = parseDouble(data);                                
                PhaseObservation pObs = PhaseObservation(obsDef.GetObservationBand(), std::move(satellite), phase);
                _Epochs.back().AddPhaseObservation(pObs);
                break;
            }
            case ObservationType::Doppler:
            {         
                double doppler = parseDouble(data);
                DopplerObservation dObs = DopplerObservation(obsDef.GetObservationBand(), std::move(satellite), doppler);
                _Epochs.back().AddDopplerObservation(dObs);
                break;
            }
            case ObservationType::RawSignalStrength:
            {   
                double snr = parseDouble(data);
                SignalStrengthObservation dObs = SignalStrengthObservation(obsDef.GetObservationBand(), std::move(satellite), snr);
                _Epochs.back().AddSnrObservation(dObs);                
                break;
            }
            case ObservationType::ReceiverChannelNumber:
            {                
                break;
            }
            default:
                break;
        }
    
        StartIndex += 16;
    }       
}

void RinexObsParser::ReadObservationTypes(std::string line)
{
    // SYS / # / OBS TYPES definition is: A1 2X,I3 13(1X,A3) [SvSystem (optional)][number of ObsTypes][Type,Band,Attribute]    
    SvSystem svSystem = SvSystem::UNKNOWN; //keep svSystem in case is empty!
    if (!std::isspace(line[0]))
    {
        svSystem = static_cast<SvSystem>(line[0]);        
    }    
    
    std::string obsDef = "";

    for (auto it = std::begin(line) + 7; it != std::end(line) && !std::isspace(*it); it += 4)    
    {
        // Extract the observation definition from the input line
        std::string obsDef(it, it + 3);

        // Convert the observation type, band and attribute
        auto obsType = static_cast<ObservationType>(obsDef[0]);
        auto obsBand = static_cast<ObservationBand>(obsDef[1]);
        auto obsAttribute = static_cast<ObservationAttribute>(obsDef[2]);

        // Add a new ObservationDefinition object        
        _ObservationDefinitions[svSystem].emplace_back(obsType, obsBand, obsAttribute); 
    }
}

void RinexObsParser::ParseLine(std::string line)
{
    switch (_RinexReaderState)
    {
        case RinexReaderState::PARSE_HEADER:
        {
            // new Epoch Found
            if (line[0] == '>')
            {
                _RinexReaderState = RinexReaderState::PARSE_EPOCH;
                this->ReadEpochHeader(line);
            }
            else if ((line.find(RINEX_VERSION_DEFINITION) != std::string::npos)) // read rinex file version
            {
                std::string str = line.substr(0, 9);
                this->_Version = ltrim(str);
            }
            else if ((line.find(RINEX_APPROX_POSITION_DEFINITION) != std::string::npos)) // approximate position
            {
                double x = parseDouble(line.substr(0, 14));
                double y = parseDouble(line.substr(14, 14));
                double z = parseDouble(line.substr(28, 14));
                
                this->_ApproximateMarkerPosition = std::make_unique<ECEF_Position>(x, y, z);
            }
            else if ((line.find(RINEX_ANTENNA_DELTA_DEFINITION) != std::string::npos)) // antenna phase center offset
            {
                double x = parseDouble(line.substr(0, 14));
                double y = parseDouble(line.substr(14, 14));
                double z = parseDouble(line.substr(28, 14));

                this->_AntennaOffset = std::make_unique<Position>(x, y, z);
            }
            else if( (line.find(RINEX_OBS_TYPE_DEFINITION) != std::string::npos) ) // read all observation types from rnx header
            {
                _RinexReaderState = RinexReaderState::PARSE_OBS_TYPES;
                this->ReadObservationTypes(line);
            }

            break;
        }        
        case RinexReaderState::PARSE_OBS_TYPES:
        {
            if ((line.find(RINEX_OBS_TYPE_DEFINITION) != std::string::npos)) // read all observation types from rnx header
            {
                this->ReadObservationTypes(line);
            }
            else
            {
                _RinexReaderState = RinexReaderState::PARSE_HEADER;
            }
            break;
        }
        case RinexReaderState::PARSE_EPOCH:
        {
            // new Epoch Found
            if (line[0] == '>')
            {                
                this->ReadEpochHeader(line);

                // Check if current Epoch is Special Event
                if (_Epochs.back().IsSpecialEvent())
                {
                    _RinexReaderState = RinexReaderState::PARSE_HEADER; // TODO AA: currently no handling for special events!
                }
            }
            else
            {
                this->ReadEpochObservation(line);
            }

            break;
        }
        default:
            break;
    }
}

void RinexObsParser::ParseFile(std::string path)
{    
    this->_RinexReaderState = RinexReaderState::PARSE_HEADER;
    RinexParser::ParseFile(path);
}

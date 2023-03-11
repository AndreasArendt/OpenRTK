//
//  RinexParser.cpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#include "RinexObservationParser.hpp"
#include "../RinexTypes/ObservationType.hpp"
#include "../RinexTypes/ObservationBand.hpp"
#include "../RinexTypes/ObservationAttribute.hpp"
#include "../Observations/CodeObservation.hpp"
#include "../Observations/DopplerObservation.hpp"
#include "../Observations/PhaseObservation.hpp"
#include "../Utils/strim.hpp"

#include <string>

#define RINEX_VERSION_DEFINITION         "RINEX VERSION / TYPE"
#define RINEX_APPROX_POSITION_DEFINITION "APPROX POSITION XYZ"
#define RINEX_ANTENNA_DELTA_DEFINITION   "ANTENNA: DELTA H/E/N"
#define RINEX_OBS_TYPE_DEFINITION        "SYS / # / OBS TYPES"

RinexObservationParser::RinexObservationParser()
{    
}

RinexObservationParser::~RinexObservationParser()
{            
    this->_Epochs.clear();
    this->_ObservationDefinitions.clear();    
    this->_AntennaOffset.reset();
    this->_ApproximateMarkerPosition.reset();
}

void RinexObservationParser::ReadEpochHeader(std::string line)
{
    //0 2    7  10 13 15 17          2931
    //> 2019 01 25 03 24 30.0000000  0 24
    int year = std::stoi( line.substr(2, 4) );
    int month = std::stoi( line.substr(7, 2) );
    int day = std::stoi( line.substr(10, 2) );
    int hour = std::stoi( line.substr(13, 2) );
    int minute = std::stoi( line.substr(16, 2) );
    double second = std::stod( line.substr(19, 10) );
    int epochFlag = std::stoi(line.substr(31, 1));
    int numberSVs = std::stoi( line.substr(33, 2) );
    
    _Epochs.emplace_back(year, month, day, hour, minute, second, epochFlag, numberSVs);
}

void RinexObservationParser::ReadEpochObservation(std::string line)
{
    SvSystem svSystem = static_cast<SvSystem>(line[0]);
    int svNumber = std::stoi(line.substr(1, 2));
        
    unsigned int StartIndex = 3; //offset of first observation
    
    std::vector<ObservationDefinition> SvObsDefinitions = _ObservationDefinitions[svSystem];

    // See 6.7 in rinex standard how observations are formatted    
    for (size_t i = 0; i < SvObsDefinitions.size(); i++)
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

        switch (SvObsDefinitions.at(i).GetObservationType())
        {            
            case ObservationType::Code: //Pseudorange
            {   
                double psuedorange = std::stod(data);                    
                CodeObservation cObs = CodeObservation(SvObsDefinitions.at(i).GetObservationBand(), svSystem, svNumber, psuedorange);
                _Epochs.back().AddCodeObservation(cObs);                
                break;
            }
            case ObservationType::Phase: //Carrierphase
            {
                double phase = std::stod(data);                                
                PhaseObservation pObs = PhaseObservation(SvObsDefinitions.at(i).GetObservationBand(), svSystem, svNumber, phase);
                _Epochs.back().AddPhaseObservation(pObs);
                break;
            }
            case ObservationType::Doppler:
            {         
                double doppler = std::stod(data);
                DopplerObservation dObs = DopplerObservation(SvObsDefinitions.at(i).GetObservationBand(), svSystem, svNumber, doppler);
                _Epochs.back().AddDopplerObservation(dObs);
                break;
            }
            case ObservationType::RawSignalStrength:
            {                
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

void RinexObservationParser::ReadObservationTypes(std::string line)
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

void RinexObservationParser::ParseLine(std::string line)
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
                double x = std::stod(line.substr(0, 14));
                double y = std::stod(line.substr(14, 14));
                double z = std::stod(line.substr(28, 14));
                
                this->_ApproximateMarkerPosition = std::make_unique<ECEF_Position>(x, y, z);
            }
            else if ((line.find(RINEX_ANTENNA_DELTA_DEFINITION) != std::string::npos)) // antenna phase center offset
            {
                double x = std::stod(line.substr(0, 14));
                double y = std::stod(line.substr(14, 14));
                double z = std::stod(line.substr(28, 14));

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
                if (_Epochs.begin()->isSpecialEvent)
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

void RinexObservationParser::ParseFile(std::string path)
{
    std::ifstream infile(path);
    
    std::string line;
    while (std::getline(infile, line))
    {
        this->ParseLine(line);
        //std::cout << line << std::endl;
    }
}

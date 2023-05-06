//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../AbstractRinexParser.hpp"
#include "../RinexTypes/Epoch/Epoch.hpp"
#include "../RinexTypes/ObservationDefinition.hpp"
#include "./RinexReaderState.hpp"
#include "../../Transformations/ECEF_Position.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

class RinexObsParser : AbstractRinexParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);    
    
    std::string _Version;
    std::vector<Epoch> _Epochs;
    std::unordered_map<SvSystem, std::vector<ObservationDefinition>> _ObservationDefinitions;
    RinexReaderState _RinexReaderState = RinexReaderState::PARSE_HEADER;
    ECEF_Position _ApproximateMarkerPosition;
    Position _AntennaOffset;
        
public:
    //getters
    std::vector<Epoch> const& Epochs() const { return _Epochs; }
    std::string const& Version() const { return _Version; }
    ECEF_Position const& ApproximateMarkerPosition() const { return _ApproximateMarkerPosition; }
    Position const& AntennaOffset() const { return _AntennaOffset; }
        
    // ctor & dtor
    RinexObsParser();
    ~RinexObsParser();
        
    //functions
    void InitParser();
};

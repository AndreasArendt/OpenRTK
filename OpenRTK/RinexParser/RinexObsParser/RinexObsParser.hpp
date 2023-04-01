//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../RinexParser.hpp"
#include "../RinexTypes/Epoch/ObsEpoch.hpp"
#include "../RinexTypes/ObservationDefinition.hpp"
#include "./RinexReaderState.hpp"
#include "../../Transformations/ECEF_Position.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

class RinexObsParser : RinexParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);
    
    std::string _Version;
    std::vector<ObsEpoch> _Epochs;
    std::unordered_map<SvSystem, std::vector<ObservationDefinition>> _ObservationDefinitions;
    RinexReaderState _RinexReaderState = RinexReaderState::PARSE_HEADER;
    std::unique_ptr<ECEF_Position> _ApproximateMarkerPosition;
    std::unique_ptr<Position> _AntennaOffset;
        
public:
    //getters
    std::vector<ObsEpoch> const& Epochs() const { return _Epochs; }
    std::string const& Version() const { return _Version; }
    std::unique_ptr<ECEF_Position> const& ApproximateMarkerPosition() const { return _ApproximateMarkerPosition; }
    std::unique_ptr<Position> const& AntennaOffset() const { return _AntennaOffset; }
        
    // ctor & dtor
    RinexObsParser();
    ~RinexObsParser();
        
    //functions
    void ParseFile(std::string path) override;
};

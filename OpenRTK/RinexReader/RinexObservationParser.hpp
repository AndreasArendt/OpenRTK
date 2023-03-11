//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../RinexTypes/RinexEpoch.hpp"
#include "../RinexTypes/ObservationDefinition.hpp"
#include "./RinexReaderState.hpp"

#include <stdio.h>
#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

class RinexObservationParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);
    
    std::vector<RinexEpoch> _Epochs;
    std::unordered_map<SvSystem, std::vector<ObservationDefinition>> _ObservationDefinitions;
    RinexReaderState _RinexReaderState = RinexReaderState::IDLE;

public:
    //getters
    std::vector<RinexEpoch> const& Epochs() const { return _Epochs; }

    // ctor & dtor
    RinexObservationParser();
    ~RinexObservationParser();
        
    //functions
    void ParseFile(std::string path);
};

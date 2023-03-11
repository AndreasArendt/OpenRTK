//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../RinexTypes/RinexEpoch.hpp"

#include <stdio.h>
#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class RinexObservationParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);
    
    std::vector<RinexEpoch> _Epochs;

public:
    //getters
    std::vector<RinexEpoch> const& Epochs() const { return _Epochs; }

    // ctor & dtor
    RinexObservationParser();
    ~RinexObservationParser();
        
    //functions
    void ParseFile(std::string path);
};

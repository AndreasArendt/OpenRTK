//
//  RinexParser.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include <stdio.h>
#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "RinexObservation.hpp"

class RinexParser {
    
private:    
    void ReadEpochHeader(std::string line);
    void ReadEpochObservation(std::string line);
    void ReadObservationTypes(std::string line);
    void ParseLine(std::string line);
    
public:
    RinexParser();
    ~RinexParser();
    
    void ParseFile(std::string path);
};

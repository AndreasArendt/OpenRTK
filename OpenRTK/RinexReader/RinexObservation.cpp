//
//  RinexObservation.cpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#include "RinexObservation.hpp"

RinexObservation::RinexObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber)
{    
    this->_ObservationBand = obsBand;
    this->_SvSystem = svSystem;
    this->_SvNumber = svNumber;
}

RinexObservation::~RinexObservation()
{
    
}

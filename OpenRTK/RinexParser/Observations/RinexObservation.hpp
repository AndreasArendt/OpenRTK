//
//  RinexObservation.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../RinexTypes/ObservationBand.hpp"
#include "../RinexTypes/Satellite.hpp"

#include <stdio.h>
#include <string>

class RinexObservation {
    private:
        ObservationBand _ObservationBand;
        Satellite _Satellite;

    public:
        RinexObservation(ObservationBand obsBand, Satellite& satellite) : _ObservationBand(obsBand), _Satellite(satellite) { }
        ~RinexObservation() { }
};
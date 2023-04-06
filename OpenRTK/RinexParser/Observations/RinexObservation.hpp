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
#include <memory>
#include <utility>

class RinexObservation {
    private:
        ObservationBand _ObservationBand;
        std::unique_ptr<Satellite> _Satellite;

    public:              
        RinexObservation() : _ObservationBand(ObservationBand::Band_Unkown), _Satellite() { }
        RinexObservation(ObservationBand obsBand, std::unique_ptr<Satellite> satellite) : _ObservationBand(obsBand), _Satellite(std::move(satellite)) { }
        ~RinexObservation() { }

        RinexObservation(const RinexObservation&) = delete;
        RinexObservation& operator=(const RinexObservation&) = delete;
};
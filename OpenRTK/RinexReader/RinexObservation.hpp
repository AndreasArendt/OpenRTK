//
//  RinexObservation.hpp
//  RinexReader
//
//  Created by Andreas Arendt on 11.06.22.
//

#pragma once

#include "../ObservationBand.hpp"
#include "../SvSystem.hpp"

#include <stdio.h>
#include <string>

class RinexObservation {
    private:
        ObservationBand _ObservationBand;
        SvSystem _SvSystem;
        int _SvNumber;

    public:
        RinexObservation(ObservationBand obsBand, SvSystem svSystem, int svNumber);
        ~RinexObservation();
};
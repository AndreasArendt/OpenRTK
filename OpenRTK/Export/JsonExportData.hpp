#pragma once

#include <string>
#include <vector>

#include "../Transformations/ECEF_Position.hpp"
#include "../RinexParser/RinexTypes/Epoch/Epoch.hpp"

struct GenericObservation
{
    double Band_1;
    double Band_2;
    double Band_5;
};

struct SatelliteObservation
{
    std::string SatelliteSystem;
    ECEF_Position ECEF_Position;
    double ClockOffset;
    double ClockDrift;
    double RelativisticError;

    GenericObservation Code;
    GenericObservation Carrier;
    GenericObservation Doppler;
    GenericObservation Snr;
};

struct SatelliteData
{
    Epoch Epoch;
    std::vector<SatelliteObservation> Observations;
};
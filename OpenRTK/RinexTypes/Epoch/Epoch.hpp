#pragma once

#include "../../Observations/CodeObservation.hpp"
#include "../../Observations/PhaseObservation.hpp"
#include "../../Observations/DopplerObservation.hpp"
#include "../../Observations/SignalStrengthObservation.hpp"

#include <vector>
#include <chrono>

class Epoch
{
	private:        
        std::chrono::system_clock::time_point _EpochTime;
        
	public:
        // getters
        
        // functions
        double ConvertEpochTimeToUTC();

        // ctor & Dtor
        Epoch(int year, int month, int day, int hour, int minute, double second);
        ~Epoch();
};




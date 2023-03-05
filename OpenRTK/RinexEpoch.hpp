#pragma once

#include "RinexReader/RinexObservation.hpp"

#include <vector>

class RinexEpoch
{
	private:
        int year;
        int month;
        int day;
        int hour;
        int minute;
        double second;
        int epochFlag;
        int numberSVs;
        
        std::vector<RinexObservation> _RinexObservations;

	public:
        bool isSpecialEvent;

        std::vector<RinexObservation> const& Observations() const { return _RinexObservations; }

		RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs);
        void AddObservation(RinexObservation& observation);
		~RinexEpoch();
};


#pragma once

#include "RinexReader/RinexObservation.hpp"

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
        
	public:
        bool isSpecialEvent;

		RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs);
        void AddObservation(RinexObservation& observation);
		~RinexEpoch();
};


#pragma once

#include "../Observations/CodeObservation.hpp"
#include "../Observations/PhaseObservation.hpp"
#include "../Observations/DopplerObservation.hpp"

#include <vector>
#include <chrono>

class RinexEpoch
{
	private:
        int _EpochFlag;
        int _NumberSVs;
        bool _IsSpecialEvent = false;

        std::chrono::system_clock::time_point _EpochTime;

        std::vector<CodeObservation> _CodeObservations;
        std::vector<PhaseObservation> _PhaseObservations;
        std::vector<DopplerObservation> _DopplerObservations;
        
	public:
        bool isSpecialEvent;
        
        // getters
        std::vector<CodeObservation> const& CodeObservations() const { return _CodeObservations; }
        std::vector<PhaseObservation> const& PhaseObservations() const { return _PhaseObservations; }
        std::vector<DopplerObservation> const& DopplerObservations() const { return _DopplerObservations; }

        // ctor & Dtor
        RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs);
        ~RinexEpoch();

        // functions
        void AddCodeObservation(CodeObservation& observation);
        void AddPhaseObservation(PhaseObservation& observation);
        void AddDopplerObservation(DopplerObservation& observation);		
};




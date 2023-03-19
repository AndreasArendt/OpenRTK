#pragma once

#include "../Observations/CodeObservation.hpp"
#include "../Observations/PhaseObservation.hpp"
#include "../Observations/DopplerObservation.hpp"
#include "../Observations/SignalStrengthObservation.hpp"

#include <vector>
#include <chrono>

class RinexEpoch
{
	private:
        int _EpochFlag;
        int _NumberSVs;
        bool _IsSpecialEvent;

        std::chrono::system_clock::time_point _EpochTime;

        std::vector<CodeObservation> _CodeObservations;
        std::vector<PhaseObservation> _PhaseObservations;
        std::vector<DopplerObservation> _DopplerObservations;
        std::vector<SignalStrengthObservation> _SnrObservations;
        
	public:
        // getters
        std::vector<CodeObservation> const& CodeObservations() const { return this->_CodeObservations; }
        std::vector<PhaseObservation> const& PhaseObservations() const { return this->_PhaseObservations; }
        std::vector<DopplerObservation> const& DopplerObservations() const { return this->_DopplerObservations; }
        std::vector<SignalStrengthObservation> const& SnrObservations() const { return this->_SnrObservations; }
        
        bool IsSpecialEvent() const { return _IsSpecialEvent; }

        // ctor & Dtor
        RinexEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs);
        ~RinexEpoch();

        // functions
        void AddCodeObservation(CodeObservation& observation);
        void AddPhaseObservation(PhaseObservation& observation);
        void AddDopplerObservation(DopplerObservation& observation);		
        void AddSnrObservation(SignalStrengthObservation& observation);

        double ConvertEpochTimeToUTC();
};




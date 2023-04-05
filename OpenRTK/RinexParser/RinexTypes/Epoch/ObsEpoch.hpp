#pragma once

#include "Epoch.hpp"
#include "../../Observations/CodeObservation.hpp"
#include "../../Observations/PhaseObservation.hpp"
#include "../../Observations/DopplerObservation.hpp"
#include "../../Observations/SignalStrengthObservation.hpp"

class ObsEpoch : public Epoch
{
private:
    int _EpochFlag;
    int _NumberSVs;
    bool _IsSpecialEvent;

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
    bool const& IsSpecialEvent() const { return _IsSpecialEvent; }

    // functions
    void AddCodeObservation(CodeObservation& observation);
    void AddPhaseObservation(PhaseObservation& observation);
    void AddDopplerObservation(DopplerObservation& observation);
    void AddSnrObservation(SignalStrengthObservation& observation);
    
    ObsEpoch(int year, int month, int day, int hour, int minute, double second, int epochFlag, int numberSVs) : 
        Epoch(year, month, day, hour, minute, second),  
        _EpochFlag(epochFlag), 
        _NumberSVs( numberSVs ), 
        _IsSpecialEvent( epochFlag >= 2 ) { }
    ~ObsEpoch() { }
};


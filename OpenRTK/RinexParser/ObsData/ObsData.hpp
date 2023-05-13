#pragma once

#include "../RinexTypes/Epoch/Epoch.hpp"
#include "../Observations/CodeObservation.hpp"
#include "../Observations/PhaseObservation.hpp"
#include "../Observations/DopplerObservation.hpp"
#include "../Observations/SignalStrengthObservation.hpp"
#include "../RinexTypes/ObservationBand.hpp"

#include <map>

class ObsData
{
protected:
	Epoch _Epoch;

private:
    int _EpochFlag;    
    bool _IsSpecialEvent;

    std::map<ObservationBand, CodeObservation> _CodeObservations;
    std::map<ObservationBand, PhaseObservation> _PhaseObservations;
    std::map<ObservationBand, DopplerObservation> _DopplerObservations;
    std::map<ObservationBand, SignalStrengthObservation> _SnrObservations;

public:
    // getters        
    std::chrono::system_clock::time_point const& EpochTime() const { return this->_Epoch.EpochTime(); }    
    double const& Utc() const { return std::chrono::duration<double>(this->_Epoch.EpochTime().time_since_epoch()).count(); }    
    std::map<ObservationBand, CodeObservation> const& CodeObservations() const { return this->_CodeObservations; }
    std::map<ObservationBand, PhaseObservation> const& PhaseObservations() const { return this->_PhaseObservations; }
    std::map<ObservationBand, DopplerObservation> const& DopplerObservations() const { return this->_DopplerObservations; }
    std::map<ObservationBand, SignalStrengthObservation> const& SnrObservations() const { return this->_SnrObservations; }
    bool const& IsSpecialEvent() const { return _IsSpecialEvent; }

    // functions
    void AddCodeObservation(ObservationBand band, double Pseudorange__m);
    void AddPhaseObservation(ObservationBand band, double Carrierphase__Cycles);
    void AddDopplerObservation(ObservationBand band, double Doppler__Hz);
    void AddSnrObservation(ObservationBand band, double SNR);

    ObsData() : _EpochFlag(-1), _IsSpecialEvent(false) { }

    ObsData(Epoch epoch, int epochFlag) :
        _Epoch(epoch),
        _EpochFlag(epochFlag),        
        _IsSpecialEvent(epochFlag >= 2) { }
    
    ~ObsData() { }
};


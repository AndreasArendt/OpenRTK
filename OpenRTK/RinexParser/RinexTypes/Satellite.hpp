#pragma once

#include "SvSystem.hpp"
#include "../../PrecisePositioning/Ephemeris/Ephemeris.hpp"
#include "../NavData/NavData.hpp"
#include "../NavData/GpsNavData.hpp"
#include "../ObsData/ObsData.hpp"

#include <string>
#include <vector>
#include <memory>

class Satellite
{
    //
    // remember to implement clone function when adding new data here!
    //

private:    
    SvSystem _SvSystem;
    int _SvNumber;
    std::vector<std::unique_ptr<NavData>> _NavigationData;    
    std::vector<ObsData> _ObservationData;
    std::vector<std::unique_ptr<Ephemeris>> _Ephemeris;

public:
    // getters
    SvSystem const& SVSystem() const { return this->_SvSystem; }
    int const& SvNumber() const { return this->_SvNumber; }
    std::vector<std::unique_ptr<NavData>> const& NavigationData() const { return this->_NavigationData; }
    std::vector<ObsData> const& ObservationData() const { return this->_ObservationData; }
    std::vector<std::unique_ptr<Ephemeris>> const& Ephemeris() const { return this->_Ephemeris; }

    // public functions    
    void addNavData(std::unique_ptr<NavData> navdata);
    void addObsData(ObsData obsdata);
    void calcEphemeris();

    // ctor & dtor    
    Satellite();
    Satellite(SvSystem svSystem, int svNumber);
    Satellite(std::string satStr);
    Satellite(const Satellite& other);
    ~Satellite();

    // Operator overloading
    Satellite& operator=(Satellite& other) noexcept;
    bool operator==(const Satellite& other) const;
    bool operator!=(const Satellite& other) const;
};
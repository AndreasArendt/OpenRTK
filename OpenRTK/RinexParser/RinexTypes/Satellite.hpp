#pragma once

#include "SvSystem.hpp"
#include "../../PrecisePositioning/Ephemeris.hpp"
#include "../NavData/NavData.hpp"
#include "../NavData/GpsNavData.hpp"

#include <string>
#include <vector>
#include <memory>

class Satellite
{
private:
    SvSystem _SvSystem;
    int _SvNumber;
    std::vector<std::unique_ptr<NavData>> _NavigationData;    
    Ephemeris _Ephemeris;

public:
    // getters
    SvSystem const& SVSystem() const { return this->_SvSystem; }
    int const& SvNumber() const { return this->_SvNumber; }
    std::vector<std::unique_ptr<NavData>> const& NavigationData() const { return this->_NavigationData; }
    Ephemeris const& Ephemeris() const { return this->_Ephemeris; }

    // public functions    
    void addNavData(std::unique_ptr<NavData> navdata);
    void calcEphimeris();

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
#pragma once

#include "SvSystem.hpp"
#include "../../PrecisePositioning/Ephemeris.hpp"
#include "../NavData/NavData.hpp"

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
    //template <typename... Args, std::enable_if_t<std::is_constructible_v<NavEpoch, Args...>, int> = 0>
    //void addNavEpoch(Args&&... args);

    // ctor & dtor
    Satellite() = default;
    Satellite(SvSystem svSystem, int svNumber);
    Satellite(std::string satStr);
    ~Satellite();
    Satellite(const Satellite& other);

    // move constructor and move assignment operator
    Satellite(Satellite&& other) noexcept;
    Satellite& operator=(Satellite&& other) noexcept;

    // Operator overloading
    bool operator==(const Satellite& other) const;
    bool operator!=(const Satellite& other) const;
};
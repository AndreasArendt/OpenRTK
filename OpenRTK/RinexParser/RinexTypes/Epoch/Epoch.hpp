#pragma once

#include <vector>
#include <chrono>

#include "../../../Utils/time.hpp"

class Epoch
{
	private:        
        std::chrono::system_clock::time_point _EpochTime;
        
        int _year;
        int _month;
        int _day;
        int _hour;
        int _minute;
        double _second;

	public:
        // getters        
        int const& Year() const { return this->_year; }
        int const& Month() const { return this->_month; }
        int const& Day() const { return this->_day; }
        int const& Hour() const { return this->_hour; }
        int const& Minute() const { return this->_minute; }
        double const& Second() const { return this->_second; }

        // functions
        double const Toc__s() const { return this->_EpochTime.time_since_epoch().count() / 1e7; }   //conversion to seconds
        
        // ctor & Dtor                
        Epoch();
        Epoch(int year, int month, int day, int hour, int minute, double second);
        ~Epoch();

        // operator overloading
        bool operator==(const Epoch& other) const;
        bool operator!=(const Epoch& other) const;
};




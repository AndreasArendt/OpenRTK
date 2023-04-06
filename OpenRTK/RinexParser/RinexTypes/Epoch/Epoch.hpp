#pragma once

#include <vector>
#include <chrono>

class Epoch
{
	private:        
        std::chrono::system_clock::time_point _EpochTime;
        
	public:
        // getters
        std::chrono::system_clock::time_point const& EpochTime() const { return this->_EpochTime; }

        // functions
        double ConvertEpochTimeToUTC();

        // ctor & Dtor                
        Epoch();
        Epoch(int year, int month, int day, int hour, int minute, double second);
        ~Epoch();

        // operator overloading
        bool operator==(const Epoch& other) const;
        bool operator!=(const Epoch& other) const;
};




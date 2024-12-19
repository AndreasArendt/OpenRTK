#pragma once

#include <vector>
#include <chrono>
#include <compare>

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
	double const PosixEpochTime__s() const { return this->_EpochTime.time_since_epoch().count() / 1e7; }   // Time of Clock (TOC)

	// ctor & Dtor                
	Epoch();
	Epoch(int year, int month, int day, int hour, int minute, double second);
	~Epoch();

	// operator overloading
	bool operator==(const Epoch& other) const;
	bool operator!=(const Epoch& other) const;
	//auto operator<=>(const Epoch& other) const;

	auto operator<=>(const Epoch& other) const
	{
		return this->PosixEpochTime__s() <=> other.PosixEpochTime__s();
	}
};

namespace std 
{
	template <>
	struct hash<Epoch> 
	{
		std::size_t operator()(const Epoch& obj) const noexcept 
		{			
			std::size_t h1 = std::hash<int>{}(obj.Year());
			std::size_t h2 = std::hash<int>{}(obj.Month());
			std::size_t h3 = std::hash<int>{}(obj.Day());
			std::size_t h4 = std::hash<int>{}(obj.Hour());
			std::size_t h5 = std::hash<int>{}(obj.Minute());
			std::size_t h6 = std::hash<double>{}(obj.Second());
			
			std::size_t combinedHash = h1;
			combinedHash = combinedHash * 31 + h2;
			combinedHash = combinedHash * 31 + h3;
			combinedHash = combinedHash * 31 + h4;
			combinedHash = combinedHash * 31 + h5;
			combinedHash = combinedHash * 31 + h6;

			return combinedHash;
		}
	};
}
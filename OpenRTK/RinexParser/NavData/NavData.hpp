#pragma once

#include "ENavOrbitNumber.hpp"
#include "../RinexTypes/Epoch/NavEpoch.hpp"

#include <memory>
#include <chrono>

class NavData
{
protected:
	virtual void AddOrbit_1(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_2(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_3(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_4(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_5(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_6(double data0, double data1, double data2, double data3) = 0;
	virtual void AddOrbit_7(double data0, double data1, double data2, double data3) = 0;

private:
	NavEpoch _NavEpoch;

public:
	// getters
	virtual std::chrono::system_clock::time_point const& EpochTime() const { return this->_NavEpoch.EpochTime(); }

	// functions
	virtual void AddClockErrors(double data0, double data1, double data2) = 0;
	void AddOrbit(ENavOrbitNumber orbitNumber, double data0, double data1, double data2, double data3);
	
	// ctor & dtor
	NavData() = default;
	NavData(int year, int month, int day, int hour, int minute, double second);			
	virtual ~NavData() = default;	

	// utils
	virtual std::unique_ptr<NavData> clone() const = 0;
};


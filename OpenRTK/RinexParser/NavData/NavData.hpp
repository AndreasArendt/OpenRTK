#pragma once

#include "ENavOrbitNumber.hpp"

#include <memory>

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

public:
	// functions
	virtual void AddClockErrors(double data0, double data1, double data2) = 0;
	void AddOrbit(ENavOrbitNumber orbitNumber, double data0, double data1, double data2, double data3);

	
	virtual std::unique_ptr<NavData> clone() const = 0;

	virtual ~NavData() = default;	
};


#pragma once

#include "NavData.hpp"

class GalileoNavData : virtual public NavData
{
private:
public:
	// Geerbt über NavData
	void AddClockErrors(double data0, double data1, double data2);
	void AddOrbit_1(double data0, double data1, double data2, double data3);
	void AddOrbit_2(double data0, double data1, double data2, double data3);
	void AddOrbit_3(double data0, double data1, double data2, double data3);
	void AddOrbit_4(double data0, double data1, double data2, double data3);
	void AddOrbit_5(double data0, double data1, double data2, double data3);
	void AddOrbit_6(double data0, double data1, double data2, double data3);
	void AddOrbit_7(double data0, double data1, double data2, double data3);

	GalileoNavData();
	~GalileoNavData();
};


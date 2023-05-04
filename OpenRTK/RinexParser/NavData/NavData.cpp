#include "NavData.hpp"

NavData::NavData(int year, int month, int day, int hour, int minute, double second) : _Epoch(year, month, day, hour, minute, second)
{
}

void NavData::AddOrbit(ENavOrbitNumber orbitNumber, double data0, double data1, double data2, double data3)
{
	switch (orbitNumber)
	{
	case ENavOrbitNumber::ORBIT_1:
		this->AddOrbit_1(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_2:
		this->AddOrbit_2(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_3:
		this->AddOrbit_3(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_4:
		this->AddOrbit_4(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_5:
		this->AddOrbit_5(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_6:
		this->AddOrbit_6(data0, data1, data2, data3);
		break;
	case ENavOrbitNumber::ORBIT_7:
		this->AddOrbit_7(data0, data1, data2, data3);
		break;
	default:
		break;
	}
}
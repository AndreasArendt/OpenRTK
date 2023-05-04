#include "GalileoNavData.hpp"

GalileoNavData::GalileoNavData(int year, int month, int day, int hour, int minute, double second) : NavData(year, month, day, hour, minute, second)
{
}

GalileoNavData::~GalileoNavData()
{

}

std::unique_ptr<NavData> GalileoNavData::clone() const
{
	return std::make_unique<GalileoNavData>(*this);
}

void GalileoNavData::AddClockErrors(double data0, double data1, double data2)
{
	_SV_ClockBias__s = data0;
	_SV_ClockDrift__sDs = data1;
	_SV_ClockDriftRate__sDs2 = data2;
}

void GalileoNavData::AddOrbit_1(double data0, double data1, double data2, double data3)
{
	_IODE_IssueOfData = data0;
	_Crs__m = data1;
	_DeltaN__radDs = data2;
	_M0__rad = data3;
}

void GalileoNavData::AddOrbit_2(double data0, double data1, double data2, double data3)
{
	_Cuc__rad = data0;
	_Eccentricity = data1;
	_Cus__rad = data2;
	_SqrtA___sqrtm = data3;
}

void GalileoNavData::AddOrbit_3(double data0, double data1, double data2, double data3)
{
	_Toe__s = data0;
	_Cic__rad = data1;
	_Omega0__rad = data2; //OMEGA_0
	_Cis__rad = data3;
}

void GalileoNavData::AddOrbit_4(double data0, double data1, double data2, double data3)
{
	_i0__rad = data0;
	_Crc__m = data1;
	_Omega__rad = data2; //omega
	_Omega_dot__radDs = data3; //OMEGA_DOT
}

void GalileoNavData::AddOrbit_5(double data0, double data1, double data2, double data3)
{
	_Idot__radDs = data0;
	_DataSources = data1;
	_GalWeek = data2;
	_Spare0 = 0.0;
}

void GalileoNavData::AddOrbit_6(double data0, double data1, double data2, double data3)
{
	_SigAccuracy__m = data0;
	_SvHealth = data1;
	_BDG_E5a_E1 = data2;
	_BDG_E5b_E1 = data3;
}

void GalileoNavData::AddOrbit_7(double data0, double data1, double data2, double data3)
{
	_TransmissiontimeOfMessage = data0;
	_Spare1 = 0.0;
	_Spare2 = 0.0;
	_Spare3 = 0.0;
}

double GalileoNavData::getGST()
{	
	double total_seconds = (this->_GalWeek * 604800) + this->_Toe__s;
	double offset_GPSTIME_seconds = 619315200; // 18 leap seconds not included (for UTC conversion!)
	double GST__s = total_seconds - offset_GPSTIME_seconds;
		
	return GST__s;
}

int yisleap(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_yday(int mon, int day, int year)
{
	static const int days[2][13] = {
		{0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
		{0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
	};
	int leap = yisleap(year);

	return days[leap][mon] + day;
}

// TOC
double GalileoNavData::getReceiverTime()
{
	int dayOfYear = get_yday(this->_Epoch.Month(), this->_Epoch.Day(), this->_Epoch.Year());

	double xy = static_cast<double>(this->_Epoch.Year());
	int id_GPS = static_cast<int>(365.25 * (xy - 1.0)) + dayOfYear - 722835;

	// Day of week:
	int idw = id_GPS % 7;

	// Number of GPS week:
	int nw = (id_GPS - idw) / 7;

	// seconds in the week:
	double t = static_cast<double>(idw) * 86400.0 + (int)this->_Epoch.Hour() * 3600 + (int)this->_Epoch.Minute() * 60 + +this->_Epoch.Second();
	return t;		
}
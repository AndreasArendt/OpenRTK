#pragma once

class Time
{
public:
	static int yisleap(int year)
	{
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	static int get_yday(int mon, int day, int year)
	{
		static const int days[2][13] = {
			{0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
			{0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
		};
		int leap = Time::yisleap(year);

		return days[leap][mon] + day;
	}
};
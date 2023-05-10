#pragma once

#include <string>

bool inline emptyOrWhiteSpace(const std::string& str)
{
	return (str.empty() || str.find_first_not_of(' ') == std::string::npos || std::all_of(str.begin(), str.end(), ::isspace));
}

double inline parseDouble(const std::string& str)
{
	if (emptyOrWhiteSpace(str))
		return 0.0;
	else
	{
		std::string modifiedStr(str);
		std::replace(modifiedStr.begin(), modifiedStr.end(), 'D', 'E'); // Replace 'D' notation
		return std::stod(modifiedStr);
	}
}

int inline parseInt(const std::string& str)
{
	if (emptyOrWhiteSpace(str))
		return 0;
	else
		return std::stoi(str);
}
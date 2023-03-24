#pragma once

#include <string>

bool inline emptyOrWhiteSpace(const std::string& str)
{
	return (str.empty() || str.find_first_not_of(' ') == std::string::npos);
}

double inline parseDouble(const std::string& str)
{
	if (emptyOrWhiteSpace(str))
		return 0.0;
	else
		return std::stod(str);
}

int inline parseInt(const std::string& str)
{
	if (emptyOrWhiteSpace(str))
		return 0;
	else
		return std::stoi(str);
}
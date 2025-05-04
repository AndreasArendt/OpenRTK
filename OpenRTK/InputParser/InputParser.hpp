#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

class InputParser
{
	using Callback = std::function<void(std::optional<const std::vector<std::string>>)>;

private:
	std::unordered_map<std::string, std::pair<Callback, int>> callbacks; // holding parameter and function with execution order
	std::unordered_map<std::string, std::string> descriptions;           // holding parameter and description
	std::unordered_map<std::string, std::vector<std::string>> parameter; // holding parameter with values

	void printHelp();

public:
	void addParameter(const std::string& param, const std::string& description, Callback callback, int executionOrder = INT32_MAX);
	bool const& hasParameter(const std::string& param) const { return this->parameter.find(param) != this->parameter.end(); }
	void parse(const std::string& input);

	// ctor & dtor
	InputParser();
};

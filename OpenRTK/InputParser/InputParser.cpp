#include "InputParser.hpp"

#include <sstream>
#include <algorithm>

InputParser::InputParser()
{
	this->addParameter("h", "Help - Print this dialog", [this](std::optional<std::vector<std::string>> args){this->printHelp();});
}

void InputParser::printHelp()
{
	std::cout << "Available parameters:" << std::endl;
	for (const auto& [param, description] : descriptions)
	{
		std::cout << "--" << param << ": " << description << std::endl;
	}
}

void InputParser::addParameter(const std::string& param, const std::string& description, Callback callback, int executionOrder)
{
    this->callbacks[param] = std::pair(callback, executionOrder);
	this->descriptions[param] = description;
}

void InputParser::parse(const std::string& input)
{
    std::istringstream iss(input);
    std::string token;
    std::string currentParam;

    while (iss >> token)
    {
        if (token.starts_with("--"))
        {
            currentParam = token.substr(2);
            this->parameter[currentParam] = {};  // initialize empty vector
        }
        else if (!currentParam.empty())
        {
            this->parameter[currentParam].push_back(token);
        }
    }

    // if parse error or no elements parsed, display help
    if (this->parameter.empty())
    {
        this->printHelp();
        return;
    }

    // Collect callbacks to be called
    std::vector<std::tuple<int, Callback, std::vector<std::string>>> orderedCallbacks;

    for (const auto& [param, values] : this->parameter)
    {
        auto it = callbacks.find(param);
        if (it != callbacks.end())
        {
            int order = it->second.second;
            const Callback& cb = it->second.first;
            orderedCallbacks.emplace_back(order, cb, values);
        }
    }

    // Sort callbacks by execution order
    std::sort(orderedCallbacks.begin(), orderedCallbacks.end(),
        [](const auto& a, const auto& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    // Execute callbacks in sorted order
    for (const auto& [order, cb, values] : orderedCallbacks)
    {
        cb(values);
    }
}
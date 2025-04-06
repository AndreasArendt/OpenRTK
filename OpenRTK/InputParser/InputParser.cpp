#include "InputParser.hpp"

#include <sstream>

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

void InputParser::addParameter(const std::string& param, const std::string& description, Callback callback)
{
	this->callbacks[param] = callback;
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
    if (this->parameter.size() == 0)
    {
        this->printHelp();
        return;
    }

    // Call callbacks if registered
    for (const auto& [param, values] : this->parameter)
    {
        if (callbacks.find(param) != callbacks.end()) 
        {            
            this->callbacks[param](values);
        }
    }
}
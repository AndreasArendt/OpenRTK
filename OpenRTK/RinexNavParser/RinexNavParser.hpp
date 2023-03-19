#pragma once

#include "../RinexTypes/IonosphericCorrection.hpp"

#include <string>
#include <vector>

class RinexNavParser
{
private:
	std::string _Version;
	std::string _Type;
	std::vector<IonosphericCorrection> _IonosphericCorrections;

	void ParseLine(std::string line);

public:
	// getters
	std::string const& Version() const { return this->_Version; }	
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }

	// ctor & dtor
	RinexNavParser();
	~RinexNavParser();

	// public function
	void ParseFile(std::string path);
};


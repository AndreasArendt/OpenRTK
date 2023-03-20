#pragma once

#include "../RinexTypes/IonosphericCorrection.hpp"
#include "../RinexTypes/TimeSystemCorrection.hpp"

#include <string>
#include "../RinexTypes/Epoch/NavEpoch.hpp"

#include <vector>

class RinexNavParser
{
private:
	std::string _Version;
	std::string _Type;
	std::vector<IonosphericCorrection> _IonosphericCorrections;
	std::vector<TimeSystemCorrection> _TimeSystemCorrections;
	std::vector<NavEpoch> _Epochs;

	bool _RinexHeaderParsed = false;

	void ParseLine(std::string line);
	void ParseIonoCorrDefinition(std::string line);
	void ParseTimeDiffDefinition(std::string line);

public:
	// getters
	std::string const& Version() const { return this->_Version; }	
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }
	std::vector<TimeSystemCorrection> const& TimeSystemCorrections() const { return this->_TimeSystemCorrections; }

	// ctor & dtor
	RinexNavParser();
	~RinexNavParser();

	// public function
	void ParseFile(std::string path);
};


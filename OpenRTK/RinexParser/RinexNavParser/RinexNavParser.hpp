#pragma once

#include "../RinexParser.hpp"
#include "../RinexTypes/IonosphericCorrection.hpp"
#include "../RinexTypes/TimeSystemCorrection.hpp"
#include "../RinexTypes/Epoch/NavEpoch.hpp"
#include "../RinexTypes/Satellite.hpp"
#include "NavEpochParsingState.hpp"

#include <string>
#include <vector>

class RinexNavParser : RinexParser
{
private:
	std::string _Version;
	std::string _Type;
	std::vector<IonosphericCorrection> _IonosphericCorrections;
	std::vector<TimeSystemCorrection> _TimeSystemCorrections;	
	std::vector<NavEpoch> _NavEpochs;

	bool _RinexHeaderParsed = false;
	NavEpochParsingState _NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;

	void ParseEoch(std::string line);
	void ParseLine(std::string line);
	void ParseIonoCorrDefinition(std::string line);
	void ParseTimeDiffDefinition(std::string line);

public:
	// getters
	std::string const& Version() const { return this->_Version; }	
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }
	std::vector<TimeSystemCorrection> const& TimeSystemCorrections() const { return this->_TimeSystemCorrections; }
	std::vector<NavEpoch> const& NavEpochs() const { return this->_NavEpochs; }

	// ctor & dtor
	RinexNavParser();
	~RinexNavParser();

	// public function
	void ParseFile(std::string path) override;
};


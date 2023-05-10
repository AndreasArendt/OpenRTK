#pragma once

#include "../AbstractRinexParser.hpp"
#include "../RinexTypes/IonosphericCorrection.hpp"
#include "../RinexTypes/TimeSystemCorrection.hpp"
#include "../RinexTypes/Satellite.hpp"
#include "NavEpochParsingState.hpp"

#include <string>
#include <vector>
#include <memory>

class RinexNavParser : virtual public AbstractRinexParser
{
private:
	std::string _Version;
	std::string _Type;
	std::vector<IonosphericCorrection> _IonosphericCorrections;
	std::vector<TimeSystemCorrection> _TimeSystemCorrections;	
			
	bool _RinexHeaderParsed = false;
	NavEpochParsingState _NavEpochParsingState = NavEpochParsingState::NavEpochParsingState_IDLE;

	ENavOrbitNumber _CurrentOrbitNumber = ENavOrbitNumber::ORBIT_UNKNOWN;	
	std::unique_ptr<NavData> _CurrentNavData;
	
	void ParseEpoch(std::string line);	
	void ParseLine(std::string line);
	void ParseIonoCorrDefinition(std::string line);
	void ParseTimeDiffDefinition(std::string line);
	void ParseOrbitData(std::string line);
	
public:
	// getters
	std::string const& Version() const { return this->_Version; }	
	std::string const& Type() const { return this->_Type; }
	std::vector<IonosphericCorrection> const& IonosphericCorrections() const { return this->_IonosphericCorrections; }
	std::vector<TimeSystemCorrection> const& TimeSystemCorrections() const { return this->_TimeSystemCorrections; }
	
	// ctor & dtor	
	RinexNavParser(std::vector<Satellite>& satellites) : AbstractRinexParser(satellites) {};
	~RinexNavParser();

	// public function
	void InitParser();
};


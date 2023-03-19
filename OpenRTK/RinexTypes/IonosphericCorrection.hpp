#pragma once

#include "IonosphericCorrectionParameter.hpp"

class IonosphericCorrection
{
private:
	IonosphericCorrectionParameter _IonosphericCorrectionParameter = IonosphericCorrectionParameter::IONOPARAM_UNKNOWN;
	double _alphabeta0 = 0.0;
	double _alphabeta1 = 0.0;
	double _alphabeta2 = 0.0;
	double _alphabeta3 = 0.0;
	char _TimeMark = ' ';

public:
	// getters
	IonosphericCorrectionParameter const& IonosphericParameter() const { return this->_IonosphericCorrectionParameter; }
	double const& alphabeta0() const { return this->_alphabeta0; }
	double const& alphabeta1() const { return this->_alphabeta1; }
	double const& alphabeta2() const { return this->_alphabeta2; }
	double const& alphabeta3() const { return this->_alphabeta3; }
	char const& TimeMark() const { return this->_TimeMark; }
		
	// public function
	void AddAlphaBeta(double ab0, double ab1, double ab2);
	void AddAlphaBeta(double ab0, double ab1, double ab2, double ab3);
	void AddTimeMark(char timemark);

	// ctor & dtor
	IonosphericCorrection(IonosphericCorrectionParameter ionoCorrectionPatam);
	~IonosphericCorrection();	

};


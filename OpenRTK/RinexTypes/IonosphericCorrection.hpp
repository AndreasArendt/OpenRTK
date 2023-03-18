#pragma once

#include "IonosphericCorrectionParameter.hpp"

class IonosphericCorrection
{
private:
	IonosphericCorrectionParameter _IonosphericCorrectionParameter;
	double _alphabeta0;
	double _alphabeta1;
	double _alphabeta2;
	double _alphabeta3;

public:
	// getters
	IonosphericCorrectionParameter const& IonosphericParameter() const { return this->_IonosphericCorrectionParameter; }
	double const& alphabeta0() const { return this->_alphabeta0; }
	double const& alphabeta1() const { return this->_alphabeta1; }
	double const& alphabeta2() const { return this->_alphabeta2; }
	double const& alphabeta3() const { return this->_alphabeta3; }
	
	// public function
	void AddAlphaBeta(double ab0, double ab1, double ab2);
	void AddAlphaBeta(double ab0, double ab1, double ab2, double ab3);

	// ctor & dtor
	IonosphericCorrection(IonosphericCorrectionParameter ionoCorrectionPatam);
	~IonosphericCorrection();	

};


#include "IonosphericCorrection.hpp"

IonosphericCorrection::IonosphericCorrection(IonosphericCorrectionParameter ionoCorrectionPatam)
{
	this->_IonosphericCorrectionParameter = ionoCorrectionPatam;
}

void IonosphericCorrection::AddAlphaBeta(double ab0, double ab1, double ab2, double ab3)
{
	this->_alphabeta0 = ab0;
	this->_alphabeta1 = ab1;
	this->_alphabeta2 = ab2;
	this->_alphabeta3 = ab3;
}

void IonosphericCorrection::AddAlphaBeta(double ab0, double ab1, double ab2)
{
	this->AddAlphaBeta(ab0, ab1, ab2, 0);
}

IonosphericCorrection::~IonosphericCorrection()
{

}
#include "IonosphericCorrection.hpp"

IonosphericCorrection::IonosphericCorrection(IonosphericCorrectionParameter ionoCorrectionPatam)
{	
	_IonosphericCorrectionParameter = ionoCorrectionPatam;
}

void IonosphericCorrection::AddAlphaBeta(double ab0, double ab1, double ab2, double ab3)
{
	_alphabeta0 = ab0;
	_alphabeta1 = ab1;
	_alphabeta2 = ab2;
	_alphabeta3 = ab3;
}

void IonosphericCorrection::AddAlphaBeta(double ab0, double ab1, double ab2)
{
	this->AddAlphaBeta(ab0, ab1, ab2, 0);
}

void IonosphericCorrection::AddTimeMark(char timemark)
{
	_TimeMark = timemark;

}

IonosphericCorrection::~IonosphericCorrection()
{

}
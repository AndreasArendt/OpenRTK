#pragma once

#include "TimeDifferenceType.hpp"

class TimeSystemCorrection
{
private:
	TimeDifferenceType _TimeDifference = TimeDifferenceType::TDIFF_UNKNOWN;
	double _a0 = 0.0;
	double _a1 = 0.0;
	int _T = 0;		// reference time for polynomial
	int _W = 0;		// reference week number	

public:
	// getters
	TimeDifferenceType const& TimeDifference() const { return this->_TimeDifference; }
	double const& a0() const { return this->_a0; }
	double const& a1() const { return this->_a1; }
	int const& T() const { return this->_T; }
	int const& W() const { return this->_W; }

	// ctor & dtor
	TimeSystemCorrection(TimeDifferenceType timeDifference, double a0, double a1, int t, int w) : _TimeDifference(timeDifference), _a0(a0), _a1(a1), _T(t), _W(w) { }
	~TimeSystemCorrection() { };
};



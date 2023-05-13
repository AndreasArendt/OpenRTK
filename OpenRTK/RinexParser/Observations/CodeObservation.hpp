#pragma once

class CodeObservation 
{
private:
	double _Pseudorange__m;

public:			
	double const& Pseudorange__m() const { return this->_Pseudorange__m; }

	CodeObservation(double pseudorange) : _Pseudorange__m(pseudorange) { }
	~CodeObservation() { }
};


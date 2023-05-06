#pragma once

class CodeObservation 
{
private:
	double _Pseudorange__m;

public:				
	CodeObservation(double pseudorange) : _Pseudorange__m(pseudorange) { }
	~CodeObservation() { }
};


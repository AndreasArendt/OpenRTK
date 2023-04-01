#pragma once

// Kepler orbit definition
class Kepler
{
private:
	double _Eccentricity;
	double _SemimajorAxis;
	double _Inclination;
	double _LongitudeOfAscendingNode;
	double _ArgumentOfPeriapsis;
	double _TrueAnomaly;

public:
	// getters
	double const& Eccentricity()& { return this->_Eccentricity;  }
	double const& SemimajorAxis()& { return this->_SemimajorAxis; }
	double const& Inclination()& { return this->_Inclination; }
	double const& LongitudeOfAscendingNode()& { return this->_LongitudeOfAscendingNode; }
	double const& ArgumentOfPeriapsis()& { return this->_ArgumentOfPeriapsis; }
	double const& TrueAnomaly()& { return this->_TrueAnomaly; }

	// ctor & dtor
	Kepler(double eccentricity, double semimajorAxis, double inclination, double longitudeOfAscendingNode, double argumentOfPeriapsis, double trueAnomaly) :
		_Eccentricity(eccentricity),
		_SemimajorAxis(semimajorAxis),
		_Inclination(inclination),
		_LongitudeOfAscendingNode(longitudeOfAscendingNode),
		_ArgumentOfPeriapsis(argumentOfPeriapsis),
		_TrueAnomaly(trueAnomaly)
	{

	}
	~Kepler(){}
};
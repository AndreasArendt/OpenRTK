#pragma once

#include "../Utils/SvSystem.hpp"
#include "../Utils/AbstractSatellite.hpp"
#include "../Utils/Epoch.hpp"
#include "../Transformations/ECEF_Position.hpp"

#include <vector>
#include <map>

struct SP3Data
{
	ECEF_Position Position_E__m;
	double Posixtime__us;
};

class SP3Satellite : public AbstractSatellite
{
private:
	int _Accuracy; //error is 2^Accuracy [mm]
	std::map<Epoch, SP3Data> _PreciseData;

public:
	// getters
	int const& Accuracy() const { return this->_Accuracy; }
	std::map<Epoch, SP3Data> const& CorrectionData() const { return this->_PreciseData; }

	//setter
	void Accuracy(int accuracy) { this->_Accuracy = accuracy; }

	// functions
	void AddCorrectionData(Epoch epoch, ECEF_Position position, double clockCorrection)
	{
		this->_PreciseData[epoch] = SP3Data{ position, clockCorrection };
	}

	// ctor & dtor
	SP3Satellite() : _Accuracy(-1) {}
	SP3Satellite(SvSystem svSystem, int svNumber) : AbstractSatellite(svSystem, svNumber), _Accuracy(0) {}
	SP3Satellite(std::string satStr) : AbstractSatellite(satStr), _Accuracy(0) {}
	SP3Satellite(const SP3Satellite& other) : AbstractSatellite(other), _Accuracy(0) {}
};
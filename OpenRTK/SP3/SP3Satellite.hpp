#pragma once

#include "../Utils/SvSystem.hpp"
#include "../Utils/AbstractSatellite.hpp"
#include "../Utils/Epoch.hpp"
#include "../Transformations/ECEF_Position.hpp"

#include <vector>
#include <map>

class SP3Satellite : public AbstractSatellite
{
private:
	int _Accuracy;
	std::map<Epoch, std::vector<std::tuple<ECEF_Position, double>>> _CorrectionData;	//position is in Kilometers, time is in microseconds

public:	
	// getters
	int const& Accuracy() const { return this->_Accuracy; }

	//setter
	void Accuracy(int accuracy) { this->_Accuracy = accuracy; }

	// functions
	void AddCorrectionData(Epoch epoch, ECEF_Position position, double clockCorrection) 
	{ 
		this->_CorrectionData[epoch].emplace_back(position, clockCorrection);
	}

	// ctor & dtor
	SP3Satellite() : _Accuracy(-1) {}
	SP3Satellite(SvSystem svSystem, int svNumber) : AbstractSatellite(svSystem, svNumber), _Accuracy(-1) {}
	SP3Satellite(std::string satStr) : AbstractSatellite(satStr), _Accuracy(-1) {}
	SP3Satellite(const SP3Satellite& other) : AbstractSatellite(other), _Accuracy(-1) {}
};
#pragma once

#include "SvSystem.hpp"
#include "ObservationType.hpp"
#include "ObservationBand.hpp"
#include "ObservationAttribute.hpp"

class ObservationDefinition
{
private:
	ObservationType _ObservationType = ObservationType::Unknown;
	ObservationBand _ObservationBand = ObservationBand::Band_Unkown;
	ObservationAttribute _ObservationAttribute = ObservationAttribute::Channel_Unknown;

public:
	// getters
	const ObservationType& GetObservationType() const { return _ObservationType; }
	const ObservationBand& GetObservationBand() const { return _ObservationBand; }
	
	// ctor & dtor
	ObservationDefinition(ObservationType obsType, ObservationBand obsBand, ObservationAttribute obsAttribute) :
		_ObservationType(obsType), _ObservationBand(obsBand), _ObservationAttribute(obsAttribute) { }
	~ObservationDefinition() { }
};


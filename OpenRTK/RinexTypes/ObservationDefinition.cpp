#include "ObservationDefinition.hpp"
#include <tuple>
#include <unordered_map>
#include <vector>

ObservationDefinition::ObservationDefinition(ObservationType obsType, ObservationBand obsBand, ObservationAttribute obsAttribute)
{
	this->_ObservationType = obsType;
	this->_ObservationBand = obsBand;
	this->_ObservationAttribute = obsAttribute;
}

ObservationDefinition::~ObservationDefinition()
{	
}

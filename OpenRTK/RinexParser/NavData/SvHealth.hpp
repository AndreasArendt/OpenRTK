#pragma once

class SvHealth
{
private:
protected:
	int _Health;

public:
	// getters
	int const& Health() const { return this->_Health; }
	
	SvHealth() : _Health(-1) {}
	~SvHealth() {}

};

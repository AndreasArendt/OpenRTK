#pragma once

class Velocity
{
private:
	double _x__mDs = 0;
	double _y__mDs = 0;
	double _z__mDs = 0;
public:
	// getters
	double const& x() const { return _x__mDs; }
	double const& y() const { return _y__mDs; }
	double const& z() const { return _z__mDs; }

	// ctor & dtor
	Velocity() {}
	Velocity(double x, double y, double z) : _x__mDs(x), _y__mDs(y), _z__mDs(z) {}
	~Velocity() {}
};
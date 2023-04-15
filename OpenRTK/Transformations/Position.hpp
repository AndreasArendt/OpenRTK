#pragma once

class Position
{
private:
	double _x__m = 0;
	double _y__m = 0;
	double _z__m = 0;
public:
	// getters
	double const& x() const { return _x__m; }
	double const& y() const { return _y__m; }
	double const& z() const { return _z__m; }

	// ctor & dtor
	Position() {};
	Position(double x, double y, double z) : _x__m(x), _y__m(y), _z__m(z) {}
	~Position() {}
};
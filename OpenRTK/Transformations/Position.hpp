#pragma once

class Position
{
private:
	double _x__m;
	double _y__m;
	double _z__m;
public:
	// getters
	double const& x() const { return _x__m; }
	double const& y() const { return _y__m; }
	double const& z() const { return _z__m; }

	// ctor & dtor
	Position(double x, double y, double z) : _x__m(x), _y__m(y), _z__m(z) {}
	~Position() {}
};
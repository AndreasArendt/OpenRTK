#pragma once

class Transformation {

private:
	Transformation() {};
	~Transformation() {};
public:
	static const double GravitationalConstant__m3Ds2;
	static const double MeanAngularVelocityOfEarth__radDs;
	static const double SpeedOfLight__mDs;
};

const double Transformation::GravitationalConstant__m3Ds2 = 3.986004418e14;
const double Transformation::MeanAngularVelocityOfEarth__radDs = 7.2921151467e10 - 5;
const double Transformation::SpeedOfLight__mDs = 299792458;
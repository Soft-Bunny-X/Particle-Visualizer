#pragma once

//#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

class Sawtooth
{
public:
	Sawtooth();
	Sawtooth(double magnitude, double period, size_t order, bool reverse, double offset);

	std::string getWave();
	std::string getDer();

	~Sawtooth();
private:
	double magnitude;
	double period;
	size_t order;
	bool reverse;
	double offset;
	const double pi{3.1415926};
};


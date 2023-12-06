#include "Sawtooth.h"

Sawtooth::Sawtooth()
	:Sawtooth(1.0, 1.0, 10, false, 0.0)
{
}

Sawtooth::Sawtooth(double magnitude, double period, size_t order, bool reverse, double offset)
	:magnitude{magnitude}, period{ period }, order{ order }, reverse{ reverse }, offset{offset}
{
}

Sawtooth::~Sawtooth()
{
}

std::string Sawtooth::getWave()
{
	std::string expression{ "" };

	expression.append(std::to_string(offset));

	for (size_t i = 0; i < order; i++) {
		if ((reverse && (i + 1) % 2 == 0) || (!reverse && i % 2 == 0)) {
			expression.append("+");
		}
		else {
			expression.append("-");
		}

		expression.append("(2*" + std::to_string(magnitude) + "/" + std::to_string(pi) + ")");
		expression.append("/" + std::to_string((double)(i + 1)));
		expression.append("*sin(2*" + std::to_string(pi) + "*" + std::to_string((double)(i + 1)) + "*t/" + std::to_string(period) + ")");
	}

	return expression;
}

std::string Sawtooth::getDer()
{
	std::string expression{ "" };

	expression.append(std::to_string(0.0));

	for (size_t i = 0; i < order; i++) {
		if ((reverse && (i + 1) % 2 == 0) || (!reverse && i % 2 == 0)) {
			expression.append("+");
		}
		else {
			expression.append("-");
		}

		expression.append("(2*" + std::to_string(magnitude) + "/" + std::to_string(pi) + ")");
		expression.append("/" + std::to_string((double)(i + 1)));
		expression.append("*cos(2*" + std::to_string(pi) + "*" + std::to_string((double)(i + 1)) + "*t/" + std::to_string(period) + ")");
		expression.append("*2*" + std::to_string(pi) + "*" + std::to_string((double)(i + 1)) + "/" + std::to_string(period));
	}

	return expression;
}
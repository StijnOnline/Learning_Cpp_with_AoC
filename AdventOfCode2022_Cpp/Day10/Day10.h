#pragma once
#include <string>
#include "../Day.h"

class Day10 : public Day {
public:
	std::string Star1(std::string& input, bool example = false);
	std::string Star2(std::string& input, bool example = false);
	int GetDay() { return 10; }
};

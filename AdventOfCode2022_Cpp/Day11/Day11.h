#pragma once
#include <string>
#include "../Day.h"

class Day11 : public Day {
public:

	static std::string Star1(std::string& input, bool example = false);
	static std::string Star2(std::string& input, bool example = false);
	int GetDay() { return 11; }
};
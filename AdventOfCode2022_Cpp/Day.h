#pragma once

#include <string>

class Day {
public:
	virtual std::string Star1(std::string& input, bool example = false) = 0;
	virtual std::string Star2(std::string& input, bool example = false) = 0;
	virtual int GetDay() = 0;
};
#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include "../Day.h"

class Day12 : public Day{
public:
	std::string Star1(std::string& input, bool example = false);
	std::string Star2(std::string& input, bool example = false);
	int GetDay() { return 12; }
};

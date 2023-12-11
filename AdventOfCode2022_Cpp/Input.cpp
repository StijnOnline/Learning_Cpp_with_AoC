#pragma once
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include "Input.h"

std::string Input::ReadTestInput(int dayNumber) {
	std::cout << "Reading TestInput\n";

	std::ifstream t("C:/Users/stijn/source/repos/AdventOfCode2022_Cpp/Day" + std::to_string(dayNumber) + "/TestInput.txt");
	if (!t.good()) {
		std::cout << "TestInput stream not good\n";
	}
	std::stringstream buffer;
	buffer << t.rdbuf() << "\n\n";
	return buffer.str();
}
std::string Input::ReadInput(int dayNumber) {
	std::cout << "Reading Input\n";
	std::ifstream t("C:/Users/stijn/source/repos/AdventOfCode2022_Cpp/Day"+ std::to_string(dayNumber) +"/Input.txt");
	if (!t.good()) {
		std::cout << "Input stream not good\n";
	}
	std::stringstream buffer;
	buffer << t.rdbuf() << "\n\n";
	return buffer.str();
}


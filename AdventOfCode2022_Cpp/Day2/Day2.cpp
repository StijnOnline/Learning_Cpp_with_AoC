#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Day2.h"

std::string Day2::Star1(std::string& input, bool example) {
	std::stringstream X(input);

	int score = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line == "") continue;

		if (line[2] == 'X') {
			score += 1;
		}
		if (line[2] == 'Y') {
			score += 2;
		}
		if (line[2] == 'Z') {
			score += 3;
		}
		if ((int)line[0] == (int)(line[2]) - 23) {//Draw
			score += 3;
		}
		if (line[0] == 'A' && line[2] == 'Y') {//Win
			score += 6;
		}
		if (line[0] == 'B' && line[2] == 'Z') {//Win
			score += 6;
		}
		if (line[0] == 'C' && line[2] == 'X') {//Win
			score += 6;
		}

	}
	return std::to_string(score);
}

std::string Day2::Star2(std::string& input, bool example) {
	std::stringstream X(input);

	int score = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line == "") continue;

		if (line[2] == 'Y') {//Draw
			score += 3 + (int)line[0] - 64;
		}
		else if (line[2] == 'Z') {//Win
			score += 6 + ((int)line[0] - 64) % 3 + 1;
		}
		else {//a = 1
			score += ((int)line[0] - 64 + 1) % 3 + 1;
		}

	}
	return std::to_string(score);
}

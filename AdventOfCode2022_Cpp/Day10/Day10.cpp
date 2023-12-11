#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include "Day10.h"


std::string Day10::Star1(std::string& input, bool example) {
	std::stringstream ss(input);
	std::string line;

	int cycle = 1;
	int X = 1;
	int sum = 0;
	std::string CRTLine(40, '.');
	while (std::getline(ss, line))
	{
		if (line[0] == 'a') {
			int toAdd = std::stoi(line.substr(5));
			if (cycle % 40 == 20) {
				sum += cycle * X;
			}
			if ((cycle + 1) % 40 == 20) {
				sum += (cycle + 1) * X;
			}
			cycle += 2;
			X += toAdd;
		}
		else {
			if (cycle % 40 == 20) {
				sum += cycle * X;
			}
			cycle++;
		}
	}

	return std::to_string(sum);
}

std::string Day10::Star2(std::string& input, bool example) {
	std::stringstream ss(input);
	std::string line;

	int cycle = 0;
	int X = 1;
	std::string CRTLine(40, ' ');
	std::string output = "\n";
	while (std::getline(ss, line))
	{
		if (line[0] == 'a') {
			int toAdd = std::stoi(line.substr(5));
			if (abs(X - cycle % 40) <= 1) {
				CRTLine[cycle % 40] = '#';
			}
			if ((cycle + 1) % 40 == 0) {
				output += CRTLine + "\n";
				CRTLine = std::string(40, ' ');;
			}
			if (abs(X - (cycle + 1) % 40) <= 1) {
				CRTLine[(cycle + 1) % 40] = '#';
			}
			cycle += 2;
			X += toAdd;
		}
		else {
			if (abs(X - cycle % 40) <= 1) {
				CRTLine[cycle % 40] = '#';
			}
			cycle++;
		}
		if (cycle % 40 == 0) {
			output += CRTLine + "\n";
			CRTLine = std::string(40, ' ');;
		}
	}

	return output;
}

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Day4.h"

static void split(std::vector<std::string>& o, std::string& s, char delimiter) {
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		o.push_back(token);
	}
}

std::string Day4::Star1(std::string& input, bool example) {
	std::stringstream X(input);

	int count = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line == "")continue;

		std::vector<std::string> ranges;
		split(ranges, line, ',');
		std::vector<std::string> minMaxesStrings;
		for (int i = 0; i < ranges.size(); i++)
		{
			split(minMaxesStrings, ranges[i], '-');
		}

		std::vector<int> minMaxes;
		for (int i = 0; i < minMaxesStrings.size(); i++)
		{
			minMaxes.push_back(stoi(minMaxesStrings[i]));
		}
		//std::cout << minMaxes[0] << minMaxes[1] << minMaxes[2] << minMaxes[3]<<std::endl;

		if (minMaxes[0] <= minMaxes[2] && minMaxes[1] >= minMaxes[3] ||
			minMaxes[0] >= minMaxes[2] && minMaxes[1] <= minMaxes[3]) {
			count += 1;
		}
	}
	return std::to_string(count);
}

std::string Day4::Star2(std::string& input, bool example) {
	std::stringstream X(input);

	int count = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line == "")continue;

		std::vector<std::string> ranges;
		split(ranges, line, ',');
		std::vector<std::string> minMaxesStrings;
		for (int i = 0; i < ranges.size(); i++)
		{
			split(minMaxesStrings, ranges[i], '-');
		}

		std::vector<int> minMaxes;
		for (int i = 0; i < minMaxesStrings.size(); i++)
		{
			minMaxes.push_back(stoi(minMaxesStrings[i]));
		}
		//std::cout << minMaxes[0] << minMaxes[1] << minMaxes[2] << minMaxes[3]<<std::endl;

		if ((minMaxes[0] >= minMaxes[2] && minMaxes[0] <= minMaxes[3]) || (minMaxes[2] >= minMaxes[0] && minMaxes[2] <= minMaxes[1])) {
			count += 1;
		}
	}
	return std::to_string(count);
}


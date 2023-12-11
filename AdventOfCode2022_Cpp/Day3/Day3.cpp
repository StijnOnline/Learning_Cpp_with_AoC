#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include "Day3.h"

std::string Day3::Star1(std::string& input, bool example) {
	std::stringstream X(input);

	int sum = 0;
	std::string line;
	std::string splita;
	std::string splitb;
	while (std::getline(X, line))
	{
		splita = line.substr(0, line.length() / 2);
		splitb = line.substr(line.length() / 2, line.length());

		for (int i = 0; i < splita.length(); i++)
		{
			if (splitb.find(splita[i]) != std::string::npos) {
				//std::cout << splita[i]<<std::endl;
				if (splita[i] >= 'a') {
					sum += splita[i] - 'a' + 1;
				}
				else {
					sum += splita[i] - 'A' + 1 + 26;
				}
				break;
			}
		}
	}
	return std::to_string(sum);
}

std::string Day3::Star2(std::string& input, bool example) {
	std::stringstream X(input);

	int sum = 0;
	std::array< std::string, 3> lines{};
	std::string line;
	for (int i = 0; std::getline(X, line); i++)
	{
		lines[i % 3] = line;
		if (i % 3 == 2) {
			bool found;
			for (int j = 0; j < lines[0].length(); j++)
			{
				if (lines[1].find(lines[0][j]) != std::string::npos) {
					if (lines[2].find(lines[0][j]) != std::string::npos) {
						//std::cout << splita[k]<<std::endl;
						if (lines[0][j] >= 'a') {
							sum += lines[0][j] - 'a' + 1;
						}
						else {
							sum += lines[0][j] - 'A' + 1 + 26;
						}
						break;
					}
				}
			}

		}
	}

	return std::to_string(sum);
}

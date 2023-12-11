#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day6.h"

static int findlast(std::string& str, char c) {
	int p = -1;
	for (int j = 0; j < str.length(); j++)
	{
		if (c == str[j]) {
			p = j;
			//Dont break, we want last
		}
	}
	return p;
}

std::string Day6::Star1(std::string& input, bool example) {

	int uniqueCounter = 0;
	std::string lastCharacters;
	for (int i = 0; i < input.length(); i++)
	{
		int p = findlast(lastCharacters, input[i]);
		if (p != -1) {
			if (lastCharacters.length() - p < uniqueCounter) {
				uniqueCounter = lastCharacters.length() - p;
				lastCharacters = lastCharacters.substr(p, lastCharacters.length() - 1);
			}
		}
		else {
			uniqueCounter++;

			if (uniqueCounter == 4) {
				return std::to_string(i + 1);
			}
		}
		lastCharacters += input[i];
		if (lastCharacters.length() > 3) {
			lastCharacters = lastCharacters.substr(1, lastCharacters.length() - 1);
		}
	}

	return "Not found";
}

std::string Day6::Star2(std::string& input, bool example) {

	int uniqueCounter = 0;
	std::string lastCharacters;
	for (int i = 0; i < input.length(); i++)
	{
		int p = findlast(lastCharacters, input[i]);
		if (p != -1) {
			if (lastCharacters.length() - p < uniqueCounter) {
				uniqueCounter = lastCharacters.length() - p;
				lastCharacters = lastCharacters.substr(p, lastCharacters.length() - 1);
			}
		}
		else {
			uniqueCounter++;

			if (uniqueCounter == 14) {
				return std::to_string(i + 1);
			}
		}
		lastCharacters += input[i];
		if (lastCharacters.length() > 13) {
			lastCharacters = lastCharacters.substr(1, lastCharacters.length() - 1);
		}
	}

	return "Not found";
}


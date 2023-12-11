#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Day1.h"

std::string Day1::Star1(std::string& input, bool example){
	std::stringstream X(input);

	int maxCalories = 0;
	int calories = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line != "") {
			calories += std::stoi(line);
		}
		else {
			if (calories > maxCalories) {
				maxCalories = calories;
			}
			calories = 0;
		}
	}
	return std::to_string(maxCalories);
}

std::string Day1::Star2(std::string& input, bool example) {
	std::stringstream X(input);

	int maxCalories1 = 0, maxCalories2 = 0, maxCalories3 = 0;
	int calories = 0;
	std::string line;
	while (std::getline(X, line))
	{
		if (line != "") {
			calories += std::stoi(line);
		}
		else {
			if (calories > maxCalories3) {
				if (calories > maxCalories2) {
					if (calories > maxCalories1) {
						maxCalories3 = maxCalories2;
						maxCalories2 = maxCalories1;
						maxCalories1 = calories;
					}
					else {
						maxCalories3 = maxCalories2;
						maxCalories2 = calories;
					}
				}
				else {
					maxCalories3 = calories;
				}
			}
			calories = 0;
		}
	}
	return std::to_string(maxCalories1 + maxCalories2 + maxCalories3);
}

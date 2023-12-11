#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include "Day14.h"

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Coordinate {
	int x;
	int y;

	bool operator==(const Coordinate& other) const {
		return x == other.x && y == other.y;
	}

	Coordinate operator+(const Coordinate& other) const {
		return { x + other.x, y + other.y };
	}
};

struct CoordinateHash {
	std::size_t operator()(const Coordinate& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
	}
};

//loads all coordinates into set and also returns the lowest y coordinate
void LoadCave(std::string& input, std::unordered_set<Coordinate, CoordinateHash>& set, int& highestY) {
	std::stringstream X(input);
	std::string line{};
	Coordinate lastCoord{ 0,0 };
	Coordinate coord{ 0,0 };
	highestY = -1;
	while (std::getline(X, line))
	{
		if (line == "")continue;
		size_t p2 = 0;
		int x, y;
		while (p2 != std::string::npos) {
			auto p = line.find(',');
			x = stoi(line.substr(p2, p - p2));
			p2 = line.find("->");
			if (p2 == std::string::npos) {
				y = stoi(line.substr(p + 1));
			}
			else {
				y = stoi(line.substr(p + 1, p2 - p - 1));
			}
			coord.x = x;
			coord.y = y;
			if (y > highestY) {
				highestY = y;
			}
			if (lastCoord.x != 0 && lastCoord.y != 0) {
				int dx = coord.x - lastCoord.x;
				int dy = coord.y - lastCoord.y;
				for (size_t i = 0; i <= abs(dx + dy); i++)
				{
					Coordinate coord2{ lastCoord.x + sgn(dx) * i, lastCoord.y + sgn(dy) * i };
					//std::cout << coord2.x << "," << coord2.y<<"\n";
					set.insert(coord2);
				}
			}
			lastCoord.x = coord.x;
			lastCoord.y = coord.y;
			if (p2 != std::string::npos) {
				line = line.substr(p2 + 2);
				p2 = 0;
			}
		}

		lastCoord.x = 0;
		lastCoord.y = 0;
	}
}

std::string Day14::Star1(std::string& input, bool example) {
	std::unordered_set<Coordinate, CoordinateHash> coordinates;
	bool debug = false;
	int lowestPoint;//Y+ is down
	LoadCave(input, coordinates, lowestPoint);
	//std::cout << "Lowest point " << lowestPoint << "\n";

	Coordinate sand{ 500,0 };
	int sandCounter{ 0 };
	while (sand.y <= lowestPoint)
	{
		if (coordinates.find(sand + Coordinate{ 0,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ 0,1 };
		}
		else if (coordinates.find(sand + Coordinate{ -1,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ -1,1 };
		}
		else if (coordinates.find(sand + Coordinate{ 1,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ 1,1 };
		}
		else {
			coordinates.insert(sand);
			sand.x = 500;
			sand.y = 0;
			++sandCounter;
		}
	}

	return std::to_string(sandCounter);
}

std::string Day14::Star2(std::string& input, bool example) {
	std::unordered_set<Coordinate, CoordinateHash> coordinates;
	bool debug = false;
	int lowestPoint;//Y+ is down
	LoadCave(input, coordinates, lowestPoint);
	lowestPoint += 2;
	//std::cout << "Lowest point " << lowestPoint << "\n";
	Coordinate sand{ 500,0 };
	int sandCounter{ 0 };
	while (true)//O_O
	{
		bool floor = sand.y == lowestPoint - 1;
		if (!floor && coordinates.find(sand + Coordinate{ 0,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ 0,1 };
		}
		else if (!floor && coordinates.find(sand + Coordinate{ -1,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ -1,1 };
		}
		else if (!floor && coordinates.find(sand + Coordinate{ 1,1 }) == coordinates.end()) {
			sand = sand + Coordinate{ 1,1 };
		}
		else if (sand.x == 500 && sand.y == 0) {
			++sandCounter;
			break;
		}else {
			coordinates.insert(sand);
			sand.x = 500;
			sand.y = 0;
			++sandCounter;
		}
	}

	return std::to_string(sandCounter);
}



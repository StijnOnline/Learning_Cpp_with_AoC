#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day18.h"
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <unordered_map> 

namespace Day_18 {


	struct Position {
		int x;
		int y;
		int z;

		Position() : x(0), y(0), z(0) {}
		Position(int x, int y, int z) : x(x), y(y), z(z) {}

		std::string to_string() {
			return "{" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "}";
		}

		bool operator==(const Position& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		Position operator+(const Position& other) const {
			return { x + other.x, y + other.y , z + other.z };
		}
	};

	struct Edge {
		Position pos;
		Edge* to;

		Edge(Position p, Edge* t) : pos(p), to(t) {}
	};

	struct PositionHash {
		std::size_t operator()(const Position& k) const {
			return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
		}
	};

	void LoadInput(std::string& input, std::unordered_set<Position, PositionHash>& positions) {
		std::stringstream X(input);
		std::string line{};
		std::vector<std::string> coords{};
		while (std::getline(X, line))
		{
			if (line == "")continue;

			boost::split(coords, line, boost::is_any_of(","));
			Position newPos{ stoi(coords[0]) ,stoi(coords[1]) ,stoi(coords[2]) };
			positions.insert(newPos);

		}
	}
	int CountExposedEdges(std::unordered_set<Position, PositionHash>& positions) {
		int c = 0;
		for (auto p : positions)
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					for (int z = -1; z <= 1; z++)
					{
						if (!((x == 0 && y == 0) || (x == 0 && z == 0) || (y == 0 && z == 0))) continue;
						if (positions.find(p + Position{ x,y,z }) == positions.end()) {
							c++;
						}
					}
				}
			}
		}
		return c;
	}

	std::vector<Position> GetAllOutsideAir(std::unordered_set<Position, PositionHash>& positions) {
		Position min{ INT_MAX,INT_MAX,INT_MAX };
		Position max{ INT_MIN,INT_MIN ,INT_MIN };

		for (auto p : positions) {
			if (p.x < min.x) { min.x = p.x; }
			if (p.y < min.y) { min.y = p.y; }
			if (p.z < min.z) { min.z = p.z; }
			if (p.x > max.x) { max.x = p.x; }
			if (p.y > max.y) { max.y = p.y; }
			if (p.z > max.z) { max.z = p.z; }
		}
		//padding
		min = min + Position{-1,-1,-1};
		max = max + Position{1,1,1};
		Position d = max + Position{ min.x,min.y,min.z };
		//std::cout << "Total Test Volume is " << d.x * d.y * d.z << "\n";
		//std::cout << "Lava is " << positions.size() << "\n";

		std::vector<Position> openSet;
		std::vector<Position> closedSet;
		openSet.push_back(min);

		//Search all outside air
		while (openSet.size() > 0)
		{
			Position p = openSet.back();
			closedSet.push_back(p);
			openSet.pop_back();

			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					for (int z = -1; z <= 1; z++)
					{
						if (!((x == 0 && y == 0) || (x == 0 && z == 0) || (y == 0 && z == 0))) continue;
						Position cp = p + Position{ x,y,z };
						if (cp.x < min.x || cp.y < min.y || cp.z < min.z || cp.x > max.x || cp.y > max.y || cp.z > max.z) continue;
						if (positions.find(cp) == positions.end() && //not lava
							find(closedSet.begin(), closedSet.end(), cp) == closedSet.end()&& //not already in closedSet
							find(openSet.begin(), openSet.end(), cp) == openSet.end()) {//not already in openSet
							openSet.push_back(cp);
						}
					}
				}
			}
		}

		std::cout << closedSet.size() << " Outside Air\n";
		return closedSet;
	}

	int CountExteriorEdges(std::unordered_set<Position, PositionHash>& positions) {
		int c = 0;
		std::vector<Position> outsideAir{};
		outsideAir = GetAllOutsideAir(positions);
		for (auto p : positions)
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					for (int z = -1; z <= 1; z++)
					{
						if (!((x == 0 && y == 0) || (x == 0 && z == 0) || (y == 0 && z == 0))) continue;
						Position cp = p + Position{ x,y,z };
						if (find(outsideAir.begin(), outsideAir.end(), cp) != outsideAir.end()) {
							++c;
						}
					}
				}
			}
		}
		return c;
	}


	std::string Day18::Star1(std::string& input, bool example) {
		std::unordered_set<Position, PositionHash> positions{};
		LoadInput(input, positions);
		auto r = CountExposedEdges(positions);
		return std::to_string(r);
	}

	std::string Day18::Star2(std::string& input, bool example) {
		std::unordered_set<Position, PositionHash> positions{};
		LoadInput(input, positions);
		auto r = CountExteriorEdges(positions);
		return std::to_string(r);
	}
}

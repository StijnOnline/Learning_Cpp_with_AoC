#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day17.h"
#include <map>
#include <boost/algorithm/string.hpp>
#include <queue>
#include <unordered_set>


struct Position {
	int x;
	int y;

	Position() : x(0), y(0) {}
	Position(int x, int y) : x(x), y(y) {}

	bool operator==(const Position& other) const {
		return x == other.x && y == other.y;
	}

	Position operator+(const Position& other) const {
		return { x + other.x, y + other.y };
	}
};

struct PositionHash {
	std::size_t operator()(const Position& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
	}
};


struct Shape {
public:
	template<typename... Args>
	Shape(Args&&... args) : positions{ std::forward<Args>(args)... } {
		UpdateWidth();
	}

	Shape(std::vector<Position> positions) {
		this->positions = positions;
		UpdateWidth();
	}

	std::vector<Position> positions;//regard bottom-left as 0,0 for ease in this problem
	int width;

	void UpdateWidth() {
		width = 0;
		for (size_t i = 0; i < positions.size(); i++)
		{
			if (positions[i].x + 1 > width) {
				width = positions[i].x + 1;
			}
		}
	}
};

class TetrisBoard {
public:
	int width = 7;
	int spawnPos = 2;//from left
	int droppedShapes{ 0 };
	int maxHeight{ 0 };
	Shape* shape{};
	Position shapePosition;

	//return false if blocked by wall
	bool TryMoveCurrentShapeHorizontal(const bool right) {
		if (!right && shapePosition.x > 0 && CheckMove(-1, 0)) {
			shapePosition.x -= 1;
			return true;
		}else if (right && shapePosition.x + shape->width < width && CheckMove(1, 0)) {
			shapePosition.x += 1;
			return true;
		}
		else {
			return false;
		}
	}

	bool CheckMove(const int x, const int y) {
		for (size_t i = 0; i < shape->positions.size(); i++)
		{
			Position p = shapePosition + shape->positions[i] + Position{ x,y };
			if (p.y < 0 || board.find(p) != board.end()) {
				return false;
			}
		}
		return true;
	}

	//return false if couldn't move due to other shapes (and also 'solidifies')
	bool TryMoveCurrentShapeDown() {
		bool free = CheckMove(0, -1);

		if (free) {
			shapePosition.y -= 1;
		}
		else {
			++droppedShapes;
			if(droppedShapes<10){
				//std::cout << "Came to rest at: " << shapePosition.x << "," << shapePosition.y << std::endl;
			}
			//add all positions to board
			for (size_t j = 0; j < shape->positions.size(); j++)
			{
				Position p = shapePosition + shape->positions[j];
				board.insert(p);		
				if (p.y + 1> maxHeight) {
					maxHeight = p.y + 1;
				}
			}
		}
		return free;
	}
	void NewShape(Shape* shape) {
		this->shape = shape;
		shapePosition.x = spawnPos;
		shapePosition.y = maxHeight + 3;
		//std::cout << "Spawn at: " << shapePosition.x << "," << shapePosition.y << std::endl;
	}
private:
	std::unordered_set<Position, PositionHash> board{};
};


//should just be one line of input
void LoadInput(std::string& input, std::vector<bool>& airflows) {
	for (size_t i = 0; i < input.length()-2; i++)//-2 because input script adds \n\n
	{
		airflows.push_back(input[i]=='>');
	}
}


std::string Day17::Star1(std::string& input, bool example) {
	bool debug = false;

	std::vector<bool> airflows{};
	LoadInput(input, airflows);
	std::vector<bool>::iterator airflow_i = airflows.begin();

	std::vector<Shape> shapes{};
	shapes.push_back(Shape(Position{ 0,0 }, Position{ 1,0 }, Position{ 2,0 }, Position{ 3,0 }));					//-
	shapes.push_back(Shape(Position{ 0,1 }, Position{ 1,0 }, Position{ 1,1 }, Position{ 1,2 }, Position{ 2,1 }));	//+
	shapes.push_back(Shape(Position{ 0,0 }, Position{ 1,0 }, Position{ 2,0 }, Position{ 2,1 }, Position{ 2,2 }));	//⅃
	shapes.push_back(Shape(Position{ 0,0 }, Position{ 0,1 }, Position{ 0,2 }, Position{ 0,3 }));					//|
	shapes.push_back(Shape(Position{ 0,0 }, Position{ 0,1 }, Position{ 1,0 }, Position{ 1,1 }));					//#
	std::vector<Shape>::iterator shapes_i = shapes.begin();

	TetrisBoard tetris;
	while (tetris.droppedShapes < 2022) {
		tetris.NewShape(&(*shapes_i));
		bool d;
		do {
			bool m = tetris.TryMoveCurrentShapeHorizontal(*airflow_i);
			//std::cout << "Move " << ((*airflow_i)?"Right":"Left") << ": " << (m?"Succes" : "Fail") << std::endl;
			//std::cout << "Pos " << tetris.shapePosition.x << "," << tetris.shapePosition.y << std::endl;

			++airflow_i;
			if (airflow_i == airflows.end()) {
				airflow_i = airflows.begin();
			}

			d = tetris.TryMoveCurrentShapeDown();
			//std::cout << "Move Down " << (d ? "Succes" : "Fail") << std::endl;
			//std::cout << "Pos " << tetris.shapePosition.x << "," << tetris.shapePosition.y << std::endl;
		} while (d);

		++shapes_i;
		if (shapes_i == shapes.end()) {
			shapes_i = shapes.begin();
		}
	}

	return std::to_string(tetris.maxHeight);
}

std::string Day17::Star2(std::string& input, bool example) {
	//TODO detect when and how often it repeats itself
	return "";
}


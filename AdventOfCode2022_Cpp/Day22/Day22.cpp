#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day22.h"
#include <cassert>

//Board coordinates are start at 1,1
class BoardLine {
public:
	int min{}; //start at 1
	int max{};
	std::vector<int> walls{};
};

class Board {
public:
	std::string instructions{};
	std::vector<BoardLine> lines{}; //item 0 => coord y=1

	//You
	int xPosition{}; //start at 1
	int yPosition{}; //start at 1
	int direction{}; //Facing is 0 for right (>), 1 for down (v), 2 for left (<), and 3 for up (^)


	void LoadInput(std::string& input) {
		std::stringstream X(input);
		std::string line{};
		while (std::getline(X, line))
		{
			if (line == "")continue;
			if (line[0] != ' ' && line[0] != '.' && line[0] != '#') {
				instructions = line;
			}
			else {
				BoardLine bLine;
				bLine.max = line.length();
				for (size_t i = 0; i < line.length(); i++)
				{
					//first non space is minimum
					if (bLine.min == 0 && line[i] != ' ') {
						bLine.min = i + 1;
					}
					if (line[i] == '#') {
						bLine.walls.emplace_back(i + 1);
					}
				}
				lines.push_back(std::move(bLine));
			}
		}
		xPosition = lines[0].min;
		yPosition = 1;
	}

	BoardLine& GetLine(int yPos) {
		return lines[yPos - 1];
	}

	//loops around board
	std::pair<int,int> GetForwardPosition() {
		if (direction == 0) {//0 for right (>)
			if (xPosition == GetLine(yPosition).max) {
				return std::make_pair(GetLine(yPosition).min, yPosition);
			}
			else {
				return std::make_pair(xPosition + 1, yPosition);
			}
		}
		if (direction == 2) {//2 for left (<)
			if (xPosition == GetLine(yPosition).min) {
				return std::make_pair(GetLine(yPosition).max, yPosition);
			}
			else {
				return std::make_pair(xPosition - 1, yPosition);
			}
		}
		if (direction == 1) {//1 for down (v)
			if (yPosition == lines.size() || xPosition < GetLine(yPosition+1).min || xPosition > GetLine(yPosition+1).max) {
				//loop other direction untill find other edge
				for (size_t i = yPosition - 1; i >= 1; i--)
				{
					if (xPosition < GetLine(i).min || xPosition > GetLine(i).max) {
						return std::make_pair(xPosition, i + 1);
					}
				}
				return std::make_pair(xPosition, 1);
			}
			else {
				return std::make_pair(xPosition, yPosition + 1);
			}
		}
		if (direction == 3) {//3 for up (^)
			if (yPosition==1 ||  xPosition < GetLine(yPosition - 1).min || xPosition > GetLine(yPosition - 1).max) {
				//loop other direction untill find other edge
				for (size_t i = yPosition + 1; i < lines.size(); i++)
				{
					if (xPosition < GetLine(i).min || xPosition > GetLine(i).max) {
						return std::make_pair(xPosition, i - 1);
					}
				}
				return std::make_pair(xPosition, lines.size());
			}
			else {
				return std::make_pair(xPosition, yPosition - 1);
			}
		}
	}

	bool IsWall(int xPos, int yPos) {
		auto line = GetLine(yPos);
		auto w = find(line.walls.begin(), line.walls.end(), xPos);
		return w != line.walls.end();
	}

	//stops at walls, loops around
	void TryMove(int steps) {
		for (size_t i = 0; i < steps; i++)
		{
			auto forward = GetForwardPosition();
			if (IsWall(forward.first,forward.second)) { break; }
			xPosition = forward.first;
			yPosition = forward.second;
			//std::cout << "Position: [" << xPosition << "," << yPosition << "]\n";
		}
	}

	void Rotate(bool right) {
		direction += right ? 1 : -1;
		direction %= 4;
		if (direction < 0) { direction += 4; }	
	}

	void ExecuteInstructions() {
		size_t numberStart=0;//
		for (size_t i = 0; i < instructions.length(); i++)
		{
			if (!std::isdigit(instructions[i])) { 
				int steps = stoi(instructions.substr(numberStart,i-numberStart));
				TryMove(steps);

				Rotate(instructions[i]=='R'); 
				numberStart = i + 1;
			}
		}
		//last move
		int steps = stoi(instructions.substr(numberStart));
		TryMove(steps);
	}

	void TestWrap() {
		//Wrap vertical edge Test
		xPosition = 9;
		yPosition = 1;
		direction = 3;
		std::pair<int, int> r = GetForwardPosition();
		assert(r.first == 9 && r.second == 12);

		//Wrap vertical edge Test
		xPosition = 9;
		yPosition = 12;
		direction = 1;
		r = GetForwardPosition();
		assert(r.first == 9 && r.second == 1);

		//Board Wrap Test Case A
		xPosition = 12;
		yPosition = 7;
		direction = 0;
		r = GetForwardPosition();
		assert(r.first == 1 && r.second == 7);

		//Board Wrap Test Case B
		xPosition = 1;
		yPosition = 7;
		direction = 2;
		r = GetForwardPosition();
		assert(r.first == 12 && r.second == 7);

		//Board Wrap Test Case C
		xPosition = 6;
		yPosition = 8;
		direction = 1;
		r = GetForwardPosition();
		assert(r.first == 6 && r.second == 5);

		//Board Wrap Test Case D
		xPosition = 6;
		yPosition = 5;
		direction = 3;
		r = GetForwardPosition();
		assert(r.first == 6 && r.second == 8);
	}
};



std::string Day22::Star1(std::string& input, bool example) {
	Board board;
	board.LoadInput(input);
	if(example){ board.TestWrap(); }	
	board.ExecuteInstructions();	

	return std::to_string(1000*board.yPosition + 4*board.xPosition + board.direction);	
}


std::string Day22::Star2(std::string& input, bool example) {
	return "";
}


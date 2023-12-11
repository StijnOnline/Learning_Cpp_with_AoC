#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day9.h"


class PosLog {
public:
	int X = 0, Y = 0;
	int count = 1;
};

class ropePiece {
public:
	int x = 0, y = 0;
};

static void LogPos(std::vector<PosLog>& log, ropePiece& ropePiece) {
	for (int i = 0; i < log.size(); i++)
	{
		if (log[i].X == ropePiece.x && log[i].Y == ropePiece.y) {
			log[i].count++;
			return;
		}
	}
	PosLog n;
	n.X = ropePiece.x;
	n.Y = ropePiece.y;
	log.push_back(n);
}

static void RopeFollow(ropePiece& headPiece, ropePiece& tailPiece) {
	//horizontal
	if (headPiece.x - tailPiece.x > 1) {
		tailPiece.x++;
		if (headPiece.y > tailPiece.y) {
			tailPiece.y++;
		}
		else if (headPiece.y < tailPiece.y) {
			tailPiece.y--;
		}
	}
	else if (tailPiece.x - headPiece.x > 1) {
		tailPiece.x--;
		if (headPiece.y > tailPiece.y) {
			tailPiece.y++;
		}
		else if (headPiece.y < tailPiece.y) {
			tailPiece.y--;
		}
	}
	//vertical
	else if (headPiece.y - tailPiece.y > 1) {
		tailPiece.y++;
		if (headPiece.x > tailPiece.x) {
			tailPiece.x++;
		}
		else if (headPiece.x < tailPiece.x) {
			tailPiece.x--;
		}
	}
	else if (tailPiece.y - headPiece.y > 1) {
		tailPiece.y--;
		if (headPiece.x > tailPiece.x) {
			tailPiece.x++;
		}
		else if (headPiece.x < tailPiece.x) {
			tailPiece.x--;
		}
	}
}

std::string Day9::Star1(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;

	std::vector<PosLog> poslog;
	int Hx = 0, Hy = 0, Tx = 0, Ty = 0;



	while (std::getline(X, line))
	{
		/*
		if (line == "")continue;
		char instruction = line[0];
		int moveCount = std::stoi(line.substr(1));
		for (int i = 0; i < moveCount; i++)
		{
			if (line[0] == 'R') {
				Hx++;
			}else if(line[0] == 'L') {
				Hx--;
			}
			else if(line[0] == 'U') {
				Hy++;
			}
			else if(line[0] == 'D') {
				Hy--;
			}
			RopeFollow(Hx, Hy, Tx, Ty);
			//std::cout << Tx << "," << Ty << "\n";
			LogPos(poslog, Tx, Ty);
		}
		*/
	}

	return std::to_string(poslog.size());
}

std::string Day9::Star2(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;

	std::vector<PosLog> poslog;
	std::vector<ropePiece> ropePieces(10);

	while (std::getline(X, line))
	{
		if (line == "")continue;
		char instruction = line[0];
		int moveCount = std::stoi(line.substr(1));
		for (int i = 0; i < moveCount; i++)
		{

			if (line[0] == 'R') {
				ropePieces[0].x++;
			}
			else if (line[0] == 'L') {
				ropePieces[0].x--;
			}
			else if (line[0] == 'U') {
				ropePieces[0].y++;
			}
			else if (line[0] == 'D') {
				ropePieces[0].y--;
			}

			for (int j = 1; j < ropePieces.size(); j++)
			{
				RopeFollow(ropePieces[j - 1], ropePieces[j]);
			}
			//std::cout << Tx << "," << Ty << "\n";
			LogPos(poslog, ropePieces[ropePieces.size() - 1]);

		}
	}

	return std::to_string(poslog.size());
}

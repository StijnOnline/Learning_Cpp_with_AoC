#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include "Day12.h"

class AStar {
public:

	AStar(std::string& input) {
		std::stringstream X(input);
		std::string line{};
		std::vector<std::string> lines{};
		while (std::getline(X, line))
		{
			if (line == "")continue;
			size_t  position = line.find('S');
			if (position != std::string::npos) {
				startPos[0] = position;
				startPos[1] = lines.size();
			}
			position = line.find('E');
			if (position != std::string::npos) {
				targetPos[0] = position;
				targetPos[1] = lines.size();
			}
			lines.push_back(line);
		}
		// we just used the vector for the dynamic size, but now we have read all we can move it to array
		map = new std::string[lines.size()]{};
		for (size_t i = 0; i < lines.size(); i++)
		{
			map[i] = std::move(lines[i]);
		}
		mapWidth = map[0].length();
		mapHeight = lines.size();
	}

	~AStar() {
		delete[] map;
	}

	class Node {
	public:
		int position[2]; //Position on the grid
		Node* parent; //Parent Node of this node

		float FScore() { //GScore + HScore
			return GScore + HScore;
		}
		int GScore; //Current Travelled Distance
		int HScore; //Distance estimated based on Heuristic

		Node(int position[2], Node* parent, int GScore, int HScore) {
			this->position[0] = position[0];
			this->position[1] = position[1];
			this->parent = parent;
			this->GScore = GScore;
			this->HScore = HScore;
		}
	};

	int h(int pos[2]) {
		return (pos[0] - targetPos[0]) + (pos[1] - targetPos[1]);
	}

	Node* getLowestFScore(std::vector<Node*>& nodes) {
		Node* lowest = nullptr;
		int lowestValue{INT_MAX};
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i]->FScore() < lowestValue) {
				lowest = nodes[i];
				lowestValue = lowest->FScore();
			}
		}

		return lowest;
	}

	bool canMove(int currentPos[2], int targetPos[2]) {
		if ((map[currentPos[1]][currentPos[0]] == 'S')) {
			return true;
		}
		else if (map[targetPos[1]][targetPos[0]] == 'E') {
			return map[currentPos[1]][currentPos[0]] >= 'y';
		} 
		return map[currentPos[1]][currentPos[0]] + 1 >= map[targetPos[1]][targetPos[0]];
	}

	//returns steps needed
	int Solve(bool star2=false) {
		std::vector<Node*> openSet{};
		Node** nodesGrid = new Node* [mapHeight * mapWidth];//pointer to a pointer array :) use pointer array to say mem
		Node* lastNode = nullptr;
		for (int i = 0; i < mapHeight * mapWidth; i++)
		{
			nodesGrid[i] = nullptr;
		}

		if(!star2){
			Node* startNode = new Node(startPos, nullptr, 0, h(startPos));
			openSet.push_back(startNode);
			nodesGrid[startPos[0] + startPos[1] * mapWidth] = startNode;
		}else{
			//add all 'a' to openSet
			//we can deduce from input that these are only the (0,x) nodes because only they have access to the rest (need via b)
			for (size_t i = 0; i < mapHeight; i++)
			{
				int pos[2]{ 0,i };
				Node* startNode = new Node(pos, nullptr, 0, h(pos));
				openSet.push_back(startNode);
				nodesGrid[pos[0] + pos[1] * mapWidth] = startNode;
			}
		}


		while (openSet.size()>0) {
			Node* currentNode = getLowestFScore(openSet);
			if ((currentNode->position[0]) == targetPos[0] && (currentNode->position[1]) == targetPos[1]) {
				lastNode = currentNode;
				if (!star2) {
					int steps = currentNode->GScore;

					//print solution nicely with arrows
					std::vector<std::string> solution{};
					for (size_t i = 0; i < mapHeight; i++)
					{
						solution.push_back(map[i]);
					}
					int lastpos[2]{ currentNode->position[0] ,currentNode->position[1] };
					while (currentNode != nullptr)
					{
						//std::cout << "Node: "<< currentNode->position[0]<<","<< currentNode->position[1] << " :"<< map[currentNode->position[1]][currentNode->position[0]] << "\n";
					
						char c{};
						if (currentNode->position[1] > lastpos[1]) c = '^';
						if (currentNode->position[1] < lastpos[1]) c = 'v';
						if (currentNode->position[0] > lastpos[0]) c = '<';
						if (currentNode->position[0] < lastpos[0]) c = '>';
						solution[currentNode->position[1]][currentNode->position[0]] = c;
						lastpos[0] = currentNode->position[0];
						lastpos[1] = currentNode->position[1];
					
						currentNode = currentNode->parent;
					}
					for (size_t i = 0; i < mapHeight; i++)
					{
						//std::cout << solution[i]<<"\n";
					}
					return steps;

				}
			}

			//remove current node
			auto position = std::find_if(openSet.begin(), openSet.end(), [currentNode](const Node* node) { return node == currentNode; }); //need to check pointer is the same, otherwise we need == operator on Node
			if (position != openSet.end()){
				openSet.erase(position);
			}
			for (short x = -1; x < 2; x++)
			{
				for (short y = -1; y < 2; y++)
				{
					if (! (x==0) ^ (y==0)) continue;

					int pos[2] = { currentNode->position[0] + x, currentNode->position[1] + y };
					if (pos[0]< 0 || pos[0] >= mapWidth || pos[1] < 0 || pos[1] >= mapHeight) continue;
					if (!canMove(currentNode->position, pos)) continue;

					Node* nextNode = nodesGrid[pos[0] + pos[1] * mapWidth];
					if (nextNode == nullptr) {
						Node* newNode = new Node(pos, currentNode, currentNode->GScore + 1, h(pos));
						openSet.push_back(newNode);
						nodesGrid[pos[0] + pos[1] * mapWidth] = newNode;
					}
					else if(currentNode->GScore + 1 + h(pos) < nextNode->FScore()){
						nextNode->GScore = currentNode->GScore + 1;
						nextNode->HScore = h(nextNode->position);
						nextNode->parent = currentNode;
					}
				}
			}

		}

		//checked all
		if (star2) {
			return lastNode->GScore;
		}
		return -1;
	}

private:
	std::string* map{};
	size_t mapHeight{};
	size_t mapWidth{};
	int startPos[2];
	int targetPos[2];
};



std::string Day12::Star1(std::string& input, bool example) {
	AStar A(input);
	return std::to_string(A.Solve());
}

std::string Day12::Star2(std::string& input, bool example) {
	AStar A(input);
	return std::to_string(A.Solve(true));
}



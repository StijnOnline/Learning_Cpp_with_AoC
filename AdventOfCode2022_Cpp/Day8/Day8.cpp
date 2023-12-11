#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day8.h"


std::string Day8::Star1(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;


	std::getline(X, line);
	int gridSize = line.length();
	const int treeCount = gridSize * gridSize;
	int* trees = new int[treeCount];
	bool* visible = new bool[treeCount];  // use for OR during calculations

	//reset stream
	X.clear();
	X.seekg(0, std::ios::beg);

	//trees into array
	int lineN = 0;
	while (std::getline(X, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			trees[lineN * gridSize + i] = line[i] - '0';
			visible[lineN * gridSize + i] = false;
		}
		lineN++;
	}

	int visibleCount = 0;
	for (int d = 0; d < 2; d++)//for both directions
	{
		for (int i = 0; i < gridSize; i++)
		{
			int j;
			int highestTree = 0;
			//lambda to get correct check pos based on direction
			auto getCheckPos = [&]() {
				return (d == 0) ? (i * gridSize + j) : (j * gridSize + i);
			};

			//left to right
			for (j = 0; j < gridSize; j++)
			{
				if (i == 0 || j == 0 || i == gridSize - 1 || j == gridSize - 1) {//edges always visible
					if (!visible[getCheckPos()]) {
						visibleCount++;
					}
					visible[getCheckPos()] = true;
				}
				if (trees[getCheckPos()] > highestTree) {
					highestTree = trees[getCheckPos()];
					if (!visible[getCheckPos()]) {
						visibleCount++;
					}
					visible[getCheckPos()] = true;
				}
			}

			if (!(i == 0 || j == 0 || i == gridSize - 1 || j == gridSize - 1)) {//dont need to check from other side if on edge, already handled in first loop
				//right to left
				int highestTree2 = 0;
				for (j = gridSize - 1; j >= 0; j--)
				{
					if (trees[getCheckPos()] > highestTree2) {
						highestTree2 = trees[getCheckPos()];
						if (!visible[getCheckPos()]) {
							visibleCount++;
						}
						visible[getCheckPos()] = true;
					}
					//if we find a tree as high as the highest, it's either the same or the highest visible from the other side and we can stop
					if (trees[getCheckPos()] == highestTree) {
						if (!visible[getCheckPos()]) {
							visibleCount++;
						}
						visible[getCheckPos()] = true;
						break;
					}
				}
			}
		}
	}

	/*std::cout << "Trees\n";
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			std::cout << (visible[i * gridSize + j] ? "x" : ".");
		}
		std::cout << "\n";
	}*/



	delete[] trees;  // don't forget to delete the array to free up memory
	delete[] visible;  // don't forget to delete the array to free up memory
	return std::to_string(visibleCount);
}

std::string Day8::Star2(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;


	std::getline(X, line);
	int gridSize = line.length();
	const int treeCount = gridSize * gridSize;
	int* trees = new int[treeCount];
	int* visScore = new int[treeCount];

	//reset stream
	X.clear();
	X.seekg(0, std::ios::beg);

	//trees into array
	int lineN = 0;
	while (std::getline(X, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			trees[lineN * gridSize + i] = line[i] - '0';
			visScore[lineN * gridSize + i] = 1;
		}
		lineN++;
	}

	int highestVisibilityScore = 0;
	for (int d = 0; d < 2; d++)//for both directions
	{
		for (int i = 0; i < gridSize; i++)
		{
			int j;
			int lastTree = 0;
			//if we keep track of trees that cause obstruction we avoid a bit of checks, not sure if that much impact but fun to implement
			std::vector<int> obstructionPos;
			std::vector<int> obstructionHeight;
			//lambda to get correct check pos based on direction
			auto getCheckPos = [&]() {
				return (d == 0) ? (i * gridSize + j) : (j * gridSize + i);
			};

			//left to right
			for (j = 0; j < gridSize; j++)
			{
				int tree = trees[getCheckPos()];
				if (tree <= lastTree) {//if this tree is smaller than last: note last tree as an obstruction
					obstructionPos.push_back(j - 1);
					obstructionHeight.push_back(lastTree);
				}
				int score = j;
				for (int o = obstructionPos.size() - 1; o >= 0 > 0; o--)//search for obstruction
				{
					if (tree <= obstructionHeight[o]) {
						score = j - obstructionPos[o];
						break;
					}
				}
				visScore[getCheckPos()] *= score;
				lastTree = tree;
			}


			obstructionPos.clear();
			obstructionHeight.clear();
			lastTree = 0;

			//right to left
			for (j = gridSize - 1; j >= 0; j--)
			{
				int tree = trees[getCheckPos()];
				if (tree <= lastTree) {//if this tree is smaller than last: note last tree as an obstruction
					obstructionPos.push_back(gridSize - 1 - j - 1);
					obstructionHeight.push_back(lastTree);
				}
				int score = gridSize - 1 - j;
				for (int o = obstructionPos.size() - 1; o >= 0; o--)//search for obstruction
				{
					if (tree <= obstructionHeight[o]) {
						score = gridSize - 1 - j - obstructionPos[o];
						break;
					}
				}
				visScore[getCheckPos()] *= score;
				if (d == 1 && visScore[getCheckPos()] > highestVisibilityScore) {
					highestVisibilityScore = visScore[getCheckPos()];
				}
				lastTree = tree;
			}

		}
	}

	/*std::cout << "Trees\n";
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			std::cout << (visible[i * gridSize + j] ? "x" : ".");
		}
		std::cout << "\n";
	}*/

	std::string info;
	for (int i = 0; i < gridSize * gridSize; i++) {
		if (visScore[i] == highestVisibilityScore) {
			info = "\nTree with height " + std::to_string(trees[i]) + " at [" + std::to_string(i / gridSize) + "," + std::to_string(i % gridSize) + "] with score of" + std::to_string(highestVisibilityScore);
			break;
		}
	}

	delete[] trees;  // don't forget to delete the array to free up memory
	delete[] visScore;  // don't forget to delete the array to free up memory
	return std::to_string(highestVisibilityScore) + info;
}

static std::string RandomInput(int gridSize) {
	srand(time(0));
	std::string TestInput = "";
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			TestInput += (rand() % 10 + '0');
		}
		TestInput += "\n";
	}
	return TestInput;
}

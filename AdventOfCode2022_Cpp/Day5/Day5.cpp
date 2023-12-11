#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day5.h"


std::string Day5::Star1(std::string& input, bool example) {
	std::stringstream X(input);

	std::string line;
	std::vector < std::vector<char>> crates;

	std::getline(X, line);
	crates.resize((line.length() + 1) / 4);

	X.clear();
	X.seekg(0, std::ios::beg);

	bool brk = false;
	//process crates
	while (std::getline(X, line))
	{
		for (int i = 1; i < line.length(); i += 4)
		{
			int stackN = i / 4;
			if (std::isdigit(line[i])) {
				brk = true;
				break;
			}
			if (line[i] != ' ') {
				crates[stackN].push_back(line[i]);
			}
		}
		if (brk) { break; }
	}

	std::getline(X, line);
	//process moves
	while (std::getline(X, line))
	{
		if (line == "")continue;
		int p = line.find("from");
		int moves = std::stoi(line.substr(5, p));
		line.erase(0, p + 4);

		p = line.find("to");
		int from = std::stoi(line.substr(0, p)) - 1;
		line.erase(0, p + 2);

		int to = std::stoi(line) - 1;

		//# of moves
		for (int i = 0; i < moves; i++)
		{
			char toMove = crates[from][0];
			crates[from].erase(crates[from].begin());//remove first
			crates[to].insert(crates[to].begin(), toMove);
		}
	}

	//result
	std::string result;
	for (int i = 0; i < crates.size(); i++)
	{
		result += crates[i][0];
	}

	return result;
}

std::string Day5::Star2(std::string& input, bool example) {
	std::stringstream X(input);

	std::string line;
	std::vector < std::vector<char>> crates;

	std::getline(X, line);
	crates.resize((line.length() + 1) / 4);

	X.clear();
	X.seekg(0, std::ios::beg);

	bool brk = false;
	//process crates
	while (std::getline(X, line))
	{
		for (int i = 1; i < line.length(); i += 4)
		{
			int stackN = i / 4;
			if (std::isdigit(line[i])) {
				brk = true;
				break;
			}
			if (line[i] != ' ') {
				crates[stackN].push_back(line[i]);
			}
		}
		if (brk) { break; }
	}

	std::getline(X, line);
	//process moves
	while (std::getline(X, line))
	{
		if (line == "")continue;
		int p = line.find("from");
		int moves = std::stoi(line.substr(5, p));
		line.erase(0, p + 4);

		p = line.find("to");
		int from = std::stoi(line.substr(0, p)) - 1;
		line.erase(0, p + 2);

		int to = std::stoi(line) - 1;

		//# of moves
		for (int i = 0; i < moves; i++)
		{
			char toMove = crates[from][0];
			crates[from].erase(crates[from].begin());//remove first
			crates[to].insert(crates[to].begin() + i, toMove);
		}
	}

	//result
	std::string result;
	for (int i = 0; i < crates.size(); i++)
	{
		result += crates[i][0];
	}

	return result;
}

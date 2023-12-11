#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include "Day11.h"

bool debug = false;
bool debugThrows{};
bool debugOperations{};
bool debugRounds{false};

class Monkey {
public:
	std::vector<std::shared_ptr<uint64_t>> items{};
	std::string operation{};
	int conditionDivisibleBy{};
	int conditionTrueGoto{};
	int conditionFalseGoto{};
	int inspectedItems{};

	//Executes the operation on its first item (and thus modifies item's value) then returns to which monkey item is thrown
	int CheckandThrow(bool star2 = false, long magicNumber = 0) {
		if (debug && debugOperations) std::cout << "	Inspects " << *items[0] << "\n";


		//OPTIMISATION LATER: assign lambda instead of parsing string every time?

		//assuming all are just 'value operator value';
		std::stringstream ss(operation);
		std::string split[3]{};
		ss >> split[0];
		if (split[0] == "old") {
			split[0] = std::to_string(*items[0]);
		}
		ss >> split[1];
		ss >> split[2];
		if (split[2] == "old") {
			split[2] = std::to_string(*items[0]);
		}

		//Do operation
		if (debug && debugOperations) std::cout << "	old: " << *items[0] << "\n";
		if (debug && debugOperations) std::cout << "	operation: " << operation << "\n";
		if (split[1] == "+") {
			*items[0] = stoi(split[0]) + stoi(split[2]);
		}
		else if (split[1] == "*") {
			*items[0] = (uint64_t)stoi(split[0]) * (uint64_t)stoi(split[2]);
		}
		if (debug && debugOperations) std::cout << "	new: " << *items[0] << "\n";
		if (!star2) {
			*items[0] /= 3;
			if (debug && debugOperations) std::cout << "	worry fell to: " << *items[0] << "\n";
		}
		else {
			*items[0] %= magicNumber;
		}

		++inspectedItems;

		if (*items[0] % conditionDivisibleBy == 0) {
			return conditionTrueGoto;
		}
		else {
			return conditionFalseGoto;
		}
	}
};

void LogMonkeyInspectCounts(std::vector<Monkey> monkeys) {
	for (size_t m = 0; m < monkeys.size(); m++)
	{
		std::cout << "Monkey " << m << " inspected " << monkeys[m].inspectedItems << " times\n";
	}
}

void GenerateMonkeys(std::string& input, std::vector<Monkey>& monkeys)
{
	std::stringstream X(input);
	std::string line;
	int lineN{};
	while (std::getline(X, line))
	{
		if (line == "")continue;

		if (line[0] == 'M') {
			lineN = 0;
			//monkeys.push_back(*(new Monkey())); bad because 'new' creates a new Monkey object on the heap and then immediately dereferences it to make a copy for the push_back function.
			monkeys.emplace_back();
		}
		else {
			if (lineN == 1) {
				line = line.substr(18) + ',';
				int end = line.find(',');
				while (end != -1) {
					monkeys.back().items.push_back(std::make_shared<uint64_t>(stoi(line.substr(0, end))));
					line.erase(line.begin(), line.begin() + end + 1);
					end = line.find(',');
				}
			}
			else if (lineN == 2) {
				monkeys.back().operation = line.substr(19);
			}
			else if (lineN == 3) {
				monkeys.back().conditionDivisibleBy = stoi(line.substr(21));
			}
			else if (lineN == 4) {
				monkeys.back().conditionTrueGoto = stoi(line.substr(28));
			}
			else if (lineN == 5) {
				monkeys.back().conditionFalseGoto = stoi(line.substr(29));
			}

		}
		++lineN;
	}
}

std::string Solve(std::string& input, bool star2 = false) {

	std::vector<Monkey> monkeys{};
	GenerateMonkeys(input, monkeys);

	long magicNumber{ 1 };
	for (size_t i = 0; i < monkeys.size(); i++)
	{
		magicNumber *= monkeys[i].conditionDivisibleBy;
	}

	int rounds{ 21 };
	if (star2) {
		rounds = 10000;
	}

	for (size_t i = 1; i <= rounds; i++)
	{
		for (size_t m = 0; m < monkeys.size(); m++)
		{
			debugOperations = star2 && i < 2;

			if (debug && debugThrows && i < 2)std::cout << "Monkey " << m << ":\n";
			while (monkeys[m].items.size() > 0) {
				int throwTo = monkeys[m].CheckandThrow(star2, magicNumber);
				if (throwTo == -1) { std::cout << "Overflow at round " << i << "\n"; i == rounds; break; }
				if (debug && debugThrows && i < 2)std::cout << "	Monkey throw " << *monkeys[m].items[0] << " to " << throwTo << "\n";
				if (throwTo == m) {
					if (debug && debugThrows)std::cout << "	MONKEY THROW TO SELF\n";
				}
				monkeys[throwTo].items.push_back(monkeys[m].items[0]);
				monkeys[m].items.erase(monkeys[m].items.begin());
			}

		}

		if (debugRounds && i % 1000 == 0 || (i) == 20) {
			std::cout << "Round " << i << "\n";
			LogMonkeyInspectCounts(monkeys);
		}
		
	}
	

	long long mostInspected{};
	long long secondmostInspected{};
	for (size_t i = 0; i < monkeys.size(); i++)
	{
		if (monkeys[i].inspectedItems > mostInspected) {
			secondmostInspected = mostInspected;
			mostInspected = monkeys[i].inspectedItems;
		}
		else if (monkeys[i].inspectedItems > secondmostInspected) {
			secondmostInspected = monkeys[i].inspectedItems;
		}
	}

	return std::to_string(mostInspected * secondmostInspected);
}

std::string Day11::Star1(std::string& input, bool example) {
	return Solve(input);
}

std::string Day11::Star2(std::string& input, bool example) {
	return Solve(input, true);

}



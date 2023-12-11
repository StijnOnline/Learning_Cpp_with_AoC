#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day21.h"
#include <boost/optional.hpp>

class Day21_Monkey {
public:
	std::string name{};
	bool hasOperation{};//true: monkey just has number false:monkey has operation
	//when operation
	std::string MonkeyA_name{};
	std::shared_ptr<Day21_Monkey> MonkeyA{};
	std::string MonkeyB_name{};
	std::shared_ptr<Day21_Monkey> MonkeyB{};
	char operation{};
	//calculated value
	boost::optional<long long> value{};
};
using Monkey = Day21_Monkey;//does this avoid naming conflict from other days?


void LoadInput(std::string& input, std::vector<std::shared_ptr<Monkey>>& monkeys) {
	std::stringstream X(input);
	std::string line{};
	while (std::getline(X, line))
	{
		if (line == "")continue;
		Monkey newMonkey{};
		newMonkey.name = line.substr(0,4);
		newMonkey.hasOperation = !isdigit(line[6]);
		if (newMonkey.hasOperation) {
			newMonkey.MonkeyA_name = line.substr(6, 4);
			newMonkey.operation = line[11];
			newMonkey.MonkeyB_name = line.substr(13, 4);
		}
		else {
			newMonkey.value = stoi(line.substr(6));
		}
		monkeys.push_back(std::make_shared<Monkey>( newMonkey));
	}
}

void FillMonkeyReferences(std::vector<std::shared_ptr<Monkey>>& monkeys) {
	//set all references to monkeys instead of just name
	for (int i = monkeys.size() - 1; i >= 0; i--)
	{
		if (monkeys[i]->hasOperation) {
			//search both monkeys at same time
			std::vector<std::shared_ptr<Monkey>>::iterator A = monkeys.end();
			std::vector<std::shared_ptr<Monkey>>::iterator B = monkeys.end();
			for (auto s = monkeys.begin(); s != monkeys.end(); ++s) {
				if ((*s)->name == monkeys[i]->MonkeyA_name) {
					monkeys[i]->MonkeyA = *s;
				}
				if ((*s)->name == monkeys[i]->MonkeyB_name) {
					monkeys[i]->MonkeyB = *s;
				}
			}
			if (monkeys[i]->MonkeyA == nullptr) {
				std::cout<< "MONKEY A REF NOT FOUND\n";
				return;
			}
			if (monkeys[i]->MonkeyB == nullptr) {
				std::cout << "MONKEY B REF NOT FOUND\n";
				return;
			}
		}
	}
}

bool hasDependencyOnHuman(std::shared_ptr<Monkey> m) {
	//if has number then false
	if (!m->hasOperation) return false;
	//if has human then true
	if (m->MonkeyA_name == "humn" || m->MonkeyB_name == "humn") return true;
	//else check child dependencies
	return hasDependencyOnHuman(m->MonkeyA) || hasDependencyOnHuman(m->MonkeyB);

};

//Assumes checks have been done
void Calculate(std::shared_ptr<Monkey> m) {
	if (m->operation == '+') {
		m->value = m->MonkeyA->value.get() + m->MonkeyB->value.get();
	}
	else if (m->operation == '-') {
		m->value = m->MonkeyA->value.get() - m->MonkeyB->value.get();
	}
	else if (m->operation == '*') {
		m->value = m->MonkeyA->value.get() * m->MonkeyB->value.get();
	}
	else if (m->operation == '/') {
		m->value = m->MonkeyA->value.get() / m->MonkeyB->value.get();
	}
};
void CalculateRecursive(std::shared_ptr<Monkey> m) {
	if (!m->hasOperation) return;
	if (m->MonkeyA->hasOperation && !m->MonkeyA->value) { CalculateRecursive(m->MonkeyA); }
	if (m->MonkeyB->hasOperation && !m->MonkeyB->value) { CalculateRecursive(m->MonkeyB); }
	if (m->MonkeyA->value && m->MonkeyB->value) {
		Calculate(m);
	}
};

std::string Day21::Star1(std::string& input, bool example) {
	std::vector<std::shared_ptr<Monkey>> unknownMonkeys{};
	LoadInput(input, unknownMonkeys);
	FillMonkeyReferences(unknownMonkeys);

	auto rootMonkey = find_if(unknownMonkeys.begin(), unknownMonkeys.end(), [](std::shared_ptr<Monkey> m) {return m->name == "root"; });
	CalculateRecursive(*rootMonkey);
	if (rootMonkey != unknownMonkeys.end()) {
		if ((*rootMonkey)->value) {
			return std::to_string( (*rootMonkey)->value.get());
		}
		else {
			return "ROOT MONKEY VALUE NOT KNOWN";
		}
	}
	else {
		return "ROOT MONKEY NOT FOUND";
	}
}


std::string Day21::Star2(std::string& input, bool example) {
	std::vector<std::shared_ptr<Monkey>> monkeys{};
	LoadInput(input, monkeys);
	FillMonkeyReferences(monkeys);


	//Do both sides have dependency on human? NO, only A for both problems:
	auto rootMonkey = find_if(monkeys.begin(), monkeys.end(), [](std::shared_ptr<Monkey> m) {return m->name == "root"; });
	/*if (hasDependencyOnHuman((*rootMonkey)->MonkeyA)) {
		std::cout << "ROOT A DEPENDENCY ON HUMAN\n";
	}
	if (hasDependencyOnHuman((*rootMonkey)->MonkeyB)) {
		std::cout << "ROOT B DEPENDENCY ON HUMAN\n";
	}*/

	//We know B isnt dependent on human, so just hard code
	CalculateRecursive((*rootMonkey)->MonkeyB);
	long long valueToGet = (*rootMonkey)->MonkeyB->value.get();
	//std::cout << "Value to Get "<< valueToGet <<"\n";
	std::shared_ptr<Monkey> currentMonkey = (*rootMonkey)->MonkeyA;

	while (currentMonkey->name != "humn")
	{

		//std::cout << currentMonkey->name << ": " << currentMonkey->MonkeyA_name << " " << currentMonkey->operation << " " << currentMonkey->MonkeyB_name << "\n";
		bool directDependencyHuman = currentMonkey->MonkeyA->name == "humn" || currentMonkey->MonkeyB->name == "humn";
		if (currentMonkey->MonkeyB->name == "humn" || (!(currentMonkey->MonkeyA->name == "humn") && !hasDependencyOnHuman(currentMonkey->MonkeyA))) {
			CalculateRecursive(currentMonkey->MonkeyA);
			if (currentMonkey->operation == '+') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value A:" << currentMonkey->MonkeyA->value.get() << "\n";
				valueToGet = valueToGet - currentMonkey->MonkeyA->value.get();
				//std::cout << currentMonkey->name << " value B should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '-') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value A:" << currentMonkey->MonkeyA->value.get() << "\n";
				valueToGet = currentMonkey->MonkeyA->value.get() - valueToGet;
				//std::cout << currentMonkey->name << " value B should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '*') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value A:" << currentMonkey->MonkeyA->value.get() << "\n";
				valueToGet = valueToGet / currentMonkey->MonkeyA->value.get();
				//std::cout << currentMonkey->name << " value B should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '/') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value A:" << currentMonkey->MonkeyA->value.get() << "\n";
				valueToGet = currentMonkey->MonkeyA->value.get() * valueToGet;
				//std::cout << currentMonkey->name << " value B should become:" << valueToGet << "\n";
			}
			currentMonkey = currentMonkey->MonkeyB;
		}
		else if (currentMonkey->MonkeyA->name == "humn" || (!(currentMonkey->MonkeyB->name == "humn") && !hasDependencyOnHuman(currentMonkey->MonkeyB))) {
			CalculateRecursive(currentMonkey->MonkeyB);
			if (currentMonkey->operation == '+') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value B:" << currentMonkey->MonkeyB->value.get() << "\n";
				valueToGet = valueToGet - currentMonkey->MonkeyB->value.get();
				//std::cout << currentMonkey->name << " value A should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '-') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value B:" << currentMonkey->MonkeyB->value.get() << "\n";
				valueToGet = currentMonkey->MonkeyB->value.get() + valueToGet;
				//std::cout << currentMonkey->name << " value A should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '*') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value B:" << currentMonkey->MonkeyB->value.get() << "\n";
				valueToGet = valueToGet / currentMonkey->MonkeyB->value.get();
				//std::cout << currentMonkey->name << " value A should become:" << valueToGet << "\n";
			}
			else if (currentMonkey->operation == '/') {
				//std::cout << currentMonkey->name << " value should become:" << valueToGet << "\n";
				//std::cout << currentMonkey->name << " value B:" << currentMonkey->MonkeyB->value.get() << "\n";
				valueToGet = currentMonkey->MonkeyB->value.get() * valueToGet;
				//std::cout << currentMonkey->name << " value A should become:" << valueToGet << "\n";
			}
			currentMonkey = currentMonkey->MonkeyA;
		}
		else {
			return "DOUBLE DEPENDENCY ON HUMAN?!?!\n";
		}
	}
	currentMonkey->value = valueToGet;
	//std::cout << currentMonkey->name << " value:" << currentMonkey->value.get() << "\n";

	//double Check
	CalculateRecursive(*rootMonkey);
	assert((*rootMonkey)->MonkeyA->value.get() == (*rootMonkey)->MonkeyB->value.get());

	return std::to_string( valueToGet);
}


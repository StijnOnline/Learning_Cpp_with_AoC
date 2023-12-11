#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day20.h"
#include <list>
#include <set>
#include <unordered_map>


void LoadInput(std::string& input, std::list<int>& numbers) {
	std::stringstream X(input);
	std::string line{};
	while (std::getline(X, line))
	{
		if (line == "")continue;
		numbers.push_back(stoi(line));
	}
}

//could be template
void advance_with_wrap(std::list<int>::iterator& it, int steps, std::list<int>& numbers) {
	int size = numbers.size();
	steps %= size; // Ensure steps is within the range of the list size
	if (steps < 0) {
		steps += size; // If steps is negative, convert it to the equivalent positive steps
	}
	for (int i = 0; i < steps; ++i) {
		++it;
		if (it == numbers.end()) {
			it = numbers.begin(); // Wrap around if the end is reached
		}
	}
}

void LogNumbers(std::list<int>& numbers) {
	std::cout << "LIST: ";
	for (int n : numbers)
	{
		std::cout << n << ",";
	}
	std::cout << std::endl;
}

bool hasDuplicate(std::list<int>& numbers) {
	std::vector<int> nums{ numbers.begin(), numbers.end() };
	size_t count = nums.size();
	if (!count)
		return false;
	std::unordered_map<int, int> tbl;
	//std::set<int> tbl;
	for (size_t i = 0; i < count; i++) {
		if (tbl.find(nums[i]) != tbl.end())
			return true;
		tbl[nums[i]] = 1;
		//tbl.insert(nums[i]);
	}
	return false;
}

void Mix(std::list<int>& numbers, bool example) {
	std::vector<std::list<int>::iterator> MixedNumbers{};//track numbers which i have already mixed
	auto current = numbers.begin();
	int size = numbers.size();
	if (example) {
		LogNumbers(numbers);
	}
	while (MixedNumbers.size() != numbers.size())
	{
		int n = *current;
		if(std::find(MixedNumbers.begin(), MixedNumbers.end(), current)==MixedNumbers.end()){
			auto insert_at = current;

			advance_with_wrap(insert_at, n, numbers);
			if (n > 0) {
				++insert_at;
			}
			if (current != insert_at) {
				current = numbers.erase(current);
				if (current == numbers.end()) {//edge case
					current = numbers.begin();
				}
				auto new_location = numbers.insert(insert_at, n);
				MixedNumbers.push_back(new_location); // Mark the number as mixed		
			}
			else {
				MixedNumbers.push_back(current); // Mark the number as mixed
			}
			if (example) {
				LogNumbers(numbers);
			}
		}else{
			advance_with_wrap(current, 1, numbers);
		}
		
	}
}

//WRONG 715,4360,  2487LOW
std::string Day20::Star1(std::string& input, bool example) {
	std::list<int> numbers{};
	LoadInput(input, numbers);
	if(hasDuplicate(numbers)){
		std::cout << "NOTE: Input has duplicate!"  << std::endl;
	}
	Mix(numbers, example);
	if(example){
		LogNumbers(numbers);
	}
	
	int ans = 0;
	auto zero = std::find(numbers.begin(),numbers.end(),0);
	advance_with_wrap(zero, 1000, numbers);
	std::cout <<"1000th number after zero:"<< *zero << std::endl;
	ans += *zero;
	advance_with_wrap(zero, 1000, numbers);
	std::cout << "2000th number after zero:" << *zero << std::endl;
	ans += *zero;
	advance_with_wrap(zero, 1000, numbers);
	std::cout << "3000th number after zero:" << *zero << std::endl;
	ans += *zero;
	return std::to_string(ans);
}

std::string Day20::Star2(std::string& input, bool example) {
	return "";
}


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include "Day13.h"

class Packet {
public:
	Packet(std::string a) {//todo move semantics or something?
		rawPacket = a.substr(1, a.length() - 2);
	}
	std::string Get(size_t index) {
		int depth{ 0 };
		size_t count{ 0 };
		size_t start_p{ 0 };
		for (size_t i = 0; i < rawPacket.length(); i++)
		{
			if (rawPacket[i] == '[') {
				++depth;
			}
			else if (rawPacket[i] == ']') {
				--depth;
				if (depth == 0 && index == count) {
					std::string r{ rawPacket.substr(start_p, i - start_p + 1) };
					return r;
				}
			}
			else if (rawPacket[i] == ',') {
				if (depth == 0) {
					if(index == count){
						std::string r{ rawPacket.substr(start_p, i- start_p) };
						return r;
					}
					start_p = i+1;
					++count;
				}
			}
		}
		//last item, return from start_pointer to end
		if(index == count){
			return rawPacket.substr(start_p);
		}
		else {
			return "";
		}
	}
private:
	std::string rawPacket;
};

int CheckPacket(std::string a, std::string b, int index=-1) {//index for debug
	bool debug = false;
	if(debug)std::cout << "Checking " << a << " and " << b << ": ";

	if (a[0] == '[' || b[0] == '[') {//one or both are packets (wrap in brackets if not packet)
		if (debug)std::cout << "GOING DEEPER\n";
		Packet A((a[0] == '[') ? a : ("[" + a + "]"));
		Packet B((b[0] == '[') ? b : ("[" + b + "]"));
		for (size_t i = 0; i < 100; i++)
		{
			std::string na = A.Get(i);
			std::string nb = B.Get(i);
			if (na == "" || nb == "") {
				if (na == "" && nb == "") return -1;//No conclusion (also for loop exit condition)
				if (na == "") return true;
				if (nb == "") return false;
			}
			int r = CheckPacket(na, nb,index);
			if (r >= 0) return r;
		}
		if (debug)std::cout << "!!!!!!!!!!!!TOO DEEP!!!!!!!!!!!!\n";
		return 0;
	}
	else {
		int ia{ stoi(a) };
		int ib{ stoi(b) };
		if (ia < ib) {
			if (debug)std::cout << " smaller GOOD\n";
			return true;
		}
		else if (ia > ib) {
			if (debug)std::cout << " bigger BAD\n";
			return false;
		}
		else {
			if (debug)std::cout << " equal \n";
			return -1;//No conclusion
		}

	}

}

std::string Day13::Star1(std::string& input, bool example) {
	bool debug = false;
	std::stringstream X(input);
	std::string line{};
	std::string pairs[2]{};
	int packets{ 0 };
	int index{ 1 };
	int answer{ 0 };
	while (std::getline(X, line))
	{
		if (line == "")continue;
		if (packets == 0) {
			pairs[0] = std::move(line);
			++packets;
		}
		else if (packets == 1) {

			if (debug)std::cout << "Pair " << index << "\n";
			pairs[1] = std::move(line);
			int r = CheckPacket(pairs[0], pairs[1], index);
			if (r) {
				if (debug)std::cout << "PACKET GOOD\n";
				answer += index;
			}
			else if(r==0){
				if (debug)std::cout << "PACKET BAD\n";
			}
			else {
				if (debug)std::cout << "!!!!!!!!!!!!PACKET UNDEFINED!!!!!!!!!!!!\n";
			}
			packets = 0;
			++index;
		}
	}
	return std::to_string(answer);
}

std::string Day13::Star2(std::string& input, bool example) {
	bool debug = false;
	std::stringstream X(input);
	std::string line{};
	std::vector<std::string> packets{};
	while (std::getline(X, line))
	{
		if (line == "")continue;
		packets.push_back(line);
	}
	packets.push_back(std::string{ "[[2]]" });
	packets.push_back(std::string{ "[[6]]" });
	std::sort(packets.begin(), packets.end(), [](const std::string& a, const std::string& b)
		{
			return CheckPacket(a,b)>0;
		});
	int a = std::find(packets.begin(), packets.end(), std::string{ "[[2]]" })- packets.begin() +1;
	int b = std::find(packets.begin(), packets.end(), std::string{ "[[6]]" })- packets.begin() +1;
	return std::to_string(a*b);
}



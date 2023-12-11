#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include <chrono>
#include "Day15.h"

struct Range {
public:
	long start, end;

	//checks if overlaps other range
	bool CheckOverlap(const Range& other) {
		return (other.start <= end && other.end >= start);
	}
	//changes this Range to include other range
	void Encapsulate(const Range& other) {
		start = std::min(start, other.start);
		end = std::max(end, other.end);
	}
};

void MergeRanges(std::vector<Range>& ranges) {
	if (ranges.size() <= 1) return;
	for (int i = ranges.size()-1; i >= 0; i--)
	{
		for (int j = ranges.size() - 2; j >= 0; j--)
		{
			if (i != j &&  ranges[j].CheckOverlap(ranges[i])) { //TODO should actually also check if ranges are NEXT to eachother since we're dealing with whole numbers
				ranges[j].Encapsulate(ranges[i]);
				ranges.erase(ranges.begin() + i);
				break;
			}
		}
	}
}

struct Sensor {
public:
	long x;
	long y;
	long d;//distance to closest beacon
};
struct Beacon {
public:
	long x;
	long y;
};

void LoadInput(std::string& input, std::vector<Sensor>& sensors, std::vector<Beacon>& beacons) {
	std::stringstream X(input);
	std::string line{};
	size_t p, p2;
	while (std::getline(X, line))
	{
		if (line == "")continue;
		p = line.find(',');
		long x = stoi(line.substr(12, p - 12));
		line = line.substr(p);
		p = line.find('=');
		p2 = line.find(':');
		long y = stoi(line.substr(p + 1, p2 - p));
		line = line.substr(p2);

		p = line.find('=');
		p2 = line.find(',');
		long x2 = stoi(line.substr(p + 1, p2 - p));
		line = line.substr(p + 1);
		p = line.find('=');
		long y2 = stoi(line.substr(p + 1));

		//dont care for now if we already found this beacon, this might be higher strength
		long r = abs(x - x2) + abs(y - y2);

		Beacon b{ x2, y2};
		beacons.push_back(std::move(b));

		Sensor s{ x, y, r };
		sensors.push_back(std::move(s));
	}
}

std::vector<Range> CheckNotBeaconCount(std::vector<Sensor>& sensors, int row, bool limitRange=false, long min=0, long max=0) {
	std::vector<Range> ranges;	
	for (size_t i = 0; i < sensors.size(); i++)
	{
		long x = sensors[i].x;
		long y = sensors[i].y;
		long d = sensors[i].d;
		long o = d - abs(row - y);
		//std::cout << x << "," << y << " found " << x2 << "," << y2 << " r=" << r << "\n";
		if (o > 0) {
			Range range;
			range.start = x - o;
			range.end = x + o;
			//std::cout << "added range " << range.start << "-" << range.end << "\n";
			ranges.push_back(range);
			MergeRanges(ranges);
		}
	}
	
	return ranges;
}

std::string Day15::Star1(std::string& input, bool example) {
	bool debug = false;
	std::vector<Sensor> sensors{};
	std::vector<Beacon> beacons{};
	LoadInput(input, sensors, beacons);
	auto r = CheckNotBeaconCount(sensors, (example ? 10 : 2000000));
	long long count = 0;
	for (size_t i = 0; i < r.size(); i++)
	{
		count += r[i].end - r[i].start;		
	}
	return std::to_string(count);
}


std::string Day15::Star2(std::string& input, bool example) {
	bool debug = false;

	std::vector<Sensor> sensors{};
	std::vector<Beacon> beacons{};
	LoadInput(input, sensors, beacons);

	int checkWidth=(example ? 20 : 4000000);
	auto start = std::chrono::steady_clock::now();
	auto lastUpdate = start;
	for (size_t i = 0; i < checkWidth; i++)
	{
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastUpdate).count() > 3) {
			std::cout << i * 100 / checkWidth << "% ("<<i<<"/"<<checkWidth<< ")\n";
			lastUpdate = std::chrono::steady_clock::now();
		}
		auto r = CheckNotBeaconCount(sensors, i, true, 0, checkWidth);
		if (r.size() > 1 || r[0].start > 0 || r[0].end < checkWidth) {//should be one range if none are missing
			std::cout << "Found missing beacon in row" << i << " in ";
			std::cout << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " Seconds\n";
			long x; 
			if (r.size() == 1 && r[0].start > 0) {
				x = 0;
			}
			else if (r.size() == 1 && r[0].end < checkWidth) {
				x = checkWidth;
			}
			else{
				x = ((r[0].end < r[1].start) ? r[0].end + 1 : r[1].end + 1);
			}
			std::cout << "Beacon at " << x << ","<<i <<"\n";
				
			return std::to_string(x * (long long)4000000 + i);
				
		}
	}
	return "";
}


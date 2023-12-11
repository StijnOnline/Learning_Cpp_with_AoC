#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day19.h"
#include <regex>

struct BluePrint {
public:
	int ID = 0;
	int ORE_ROBOT_ORE_COST = 0;
	int CLAY_ROBOT_ORE_COST = 0;
	int OBSIDIAN_ROBOT_ORE_COST = 0;
	int OBSIDIAN_ROBOT_CLAY_COST = 0;
	int GEODE_ROBOT_ORE_COST = 0;
	int GEODE_ROBOT_OBSIDIAN_COST = 0;
};

//Regex to match all numbers make it easy
void LoadInput(std::string& input, std::vector<BluePrint>& blueprints, int limit = INT_MAX) {
	std::stringstream X(input);
	std::string line{};
	std::vector<std::string> coords{};
	while (std::getline(X, line) && blueprints.size()<limit)
	{
		if (line == "")continue;
		std::regex e("\\d+");
		std::smatch match;
		std::vector<int> numbers{};
		while (std::regex_search(line, match, e)) {
			for (auto x : match) {
				numbers.push_back(std::stoi(x));
			}
			line = match.suffix().str();
		}

		blueprints.push_back(BluePrint{ numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6] });

	}
}

namespace {
	class Node {
	public:
		enum Action { Wait, MakeOreRobot, MakeClayRobot, MakeObsidianRobot, MakeGeodeRobot };
		std::vector<Action> actions{};
		int timeLeft = 0;

		int ore = 0;
		int clay = 0;
		int obsidian = 0;
		int geodes = 0;

		int ore_robots = 1;
		int clay_robots = 0;
		int obsidian_robots = 0;
		int geodes_robots = 0;
		int TotalRobots() {
			return ore_robots + clay_robots + obsidian_robots + geodes_robots;
		}


		void TickTime(int ticks = 1) {
			for (size_t i = 0; i < ticks; i++)
			{
				actions.push_back(Node::Action::Wait);
			}
			timeLeft -= ticks;
			ore += ore_robots * ticks;
			clay += clay_robots * ticks;
			obsidian += obsidian_robots * ticks;
			geodes += geodes_robots * ticks;
		}
	};
}

bool CantCatchUp(Node& n, Node& bestNode) {
	int bestMax = bestNode.geodes + bestNode.geodes_robots * bestNode.timeLeft;
	int nMax = n.geodes + n.geodes_robots * n.timeLeft + (n.timeLeft - 1) * n.timeLeft / 2;
	return nMax < bestMax;
}


//some kind of BFS so we can cull paths that are behind
int CalcMaxGeodes(BluePrint& blueprint, int time) {


	std::vector<Node> nodes;
	nodes.emplace_back();
	nodes[0].timeLeft = time;
	Node bestNode;
	int pathsChecked = 0;
	int outpaced = 0;

	//loop through all possible strategies
	while (nodes.size() > 0) {
		{
			Node n = nodes.back();
			nodes.pop_back();

			if (n.timeLeft <= 0) {
				if (n.geodes > bestNode.geodes) {
					bestNode = n;
				}
				pathsChecked++;
				continue;
			}

			if (CantCatchUp(n, bestNode)) {
				outpaced++;
				pathsChecked++;
				continue;
			}

			//Wait till end
			Node w = n;
			w.TickTime(w.timeLeft);
			if (w.geodes > bestNode.geodes) {
				bestNode = w;
			}
			pathsChecked++;

			//MakeOreRobot
			w = n;
			int ticks = 0;
			if (w.ore < blueprint.ORE_ROBOT_ORE_COST) {
				ticks = (blueprint.ORE_ROBOT_ORE_COST - w.ore + w.ore_robots - 1) / w.ore_robots;
			}
			w.TickTime(ticks + 1);
			w.ore -= blueprint.ORE_ROBOT_ORE_COST;
			w.ore_robots += 1;//this robot is done after this minute
			if (w.timeLeft > 0 && w.ore_robots <= std::max( std::max( blueprint.CLAY_ROBOT_ORE_COST , blueprint.OBSIDIAN_ROBOT_ORE_COST ),blueprint.GEODE_ROBOT_ORE_COST)) {
				w.actions.push_back(Node::Action::MakeOreRobot);
				nodes.push_back(w);
			}

			//MakeClayRobot
			w = n;
			ticks = 0;
			if (w.ore < blueprint.CLAY_ROBOT_ORE_COST) {
				ticks = (blueprint.CLAY_ROBOT_ORE_COST - w.ore + w.ore_robots - 1) / w.ore_robots;
			}
			w.TickTime(ticks + 1);
			w.ore -= blueprint.CLAY_ROBOT_ORE_COST;
			w.clay_robots += 1;//this robot is done after this minute
			if (w.timeLeft > 0 && w.clay_robots <= blueprint.OBSIDIAN_ROBOT_CLAY_COST) {
				w.actions.push_back(Node::Action::MakeClayRobot);
				nodes.push_back(w);
			}

			//MakeObsidianRobot
			w = n;
			if (w.clay_robots > 0) {
				ticks = 0;
				if (w.ore < blueprint.OBSIDIAN_ROBOT_ORE_COST || w.clay < blueprint.OBSIDIAN_ROBOT_CLAY_COST) {
					ticks = std::max((blueprint.OBSIDIAN_ROBOT_ORE_COST - w.ore + w.ore_robots - 1) / w.ore_robots, (blueprint.OBSIDIAN_ROBOT_CLAY_COST - w.clay + w.clay_robots - 1) / w.clay_robots);
				}
				w.TickTime(ticks + 1);
				w.ore -= blueprint.OBSIDIAN_ROBOT_ORE_COST;
				w.clay -= blueprint.OBSIDIAN_ROBOT_CLAY_COST;
				w.obsidian_robots += 1;//this robot is done after this minute
				if (w.timeLeft > 0 && w.obsidian_robots <= blueprint.GEODE_ROBOT_OBSIDIAN_COST) {
					w.actions.push_back(Node::Action::MakeObsidianRobot);
					nodes.push_back(w);
				}
			}

			//MakeGeodeRobot
			w = n;
			if (w.obsidian_robots > 0) {
				ticks = 0;
				if (w.ore < blueprint.GEODE_ROBOT_ORE_COST || w.obsidian < blueprint.GEODE_ROBOT_OBSIDIAN_COST) {
					ticks = std::max((blueprint.GEODE_ROBOT_ORE_COST - w.ore + w.ore_robots - 1) / w.ore_robots, (blueprint.GEODE_ROBOT_OBSIDIAN_COST - w.obsidian + w.obsidian_robots - 1) / w.obsidian_robots);
				}
				w.TickTime(ticks + 1);
				w.ore -= blueprint.GEODE_ROBOT_ORE_COST;
				w.obsidian -= blueprint.GEODE_ROBOT_OBSIDIAN_COST;
				w.geodes_robots += 1;//this robot is done after this minute

				if (w.timeLeft > 0) {
					w.actions.push_back(Node::Action::MakeGeodeRobot);
					nodes.push_back(w);
				}
			}


		}
	}
	std::cout << "Blueprint " << blueprint.ID << ": ";
	std::cout << " Paths Checked: " << pathsChecked;
	std::cout << " Geodes: " << bestNode.geodes;
	std::cout << " (Trimmed outpaced: " << outpaced << ")\n";
	return bestNode.geodes;
}

std::string Day19::Star1(std::string& input, bool example) {
	std::vector<BluePrint> blueprints{};
	LoadInput(input, blueprints);
	int total = 0;
	for (auto b : blueprints)
	{
		int geodes = CalcMaxGeodes(b,24);;
		total += b.ID * geodes;
	}
	return std::to_string(total);
}

std::string Day19::Star2(std::string& input, bool example) {
	std::vector<BluePrint> blueprints{};
	LoadInput(input, blueprints,3);
	int total = 1;
	for (auto b : blueprints)
	{
		int geodes = CalcMaxGeodes(b, 32);;
		total *= geodes;
	}
	return std::to_string(total);
}


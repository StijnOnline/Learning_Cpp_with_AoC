#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Day7.h"

class dir;
class file {
public:
	std::string name;
	int size;
	dir* parentDir;


	file(std::string name, int size) {
		this->name = name;
		this->size = size;
	}
};
class dir {
public:
	std::string name;
	dir* parentDir;
	std::vector<std::shared_ptr<dir>> dirs;
	std::vector<file> files;
	int childSize;

	dir(std::string name) {
		this->name = name;
		this->childSize = 0;
	}

	void addfile(file f) {
		f.parentDir = this;
		if (parentDir) {
			parentDir->childSizeChanged(childSize, childSize + f.size);
		}
		childSize += f.size;
		files.push_back(f);
	}

	void adddir(std::shared_ptr<dir> d) {
		d->parentDir = this;
		dirs.push_back(d);
	}

	std::shared_ptr<dir> getdir(std::string name) {
		for (int i = 0; i < dirs.size(); i++)
		{
			if (dirs[i]->name == name) {
				return dirs[i];
			}
		}
		return NULL;
	}

	void childSizeChanged(int fromSize, int toSize) {
		if (parentDir) {
			parentDir->childSizeChanged(childSize, childSize - fromSize + toSize);
		}
		childSize = childSize - fromSize + toSize;
	}

};



std::string Day7::Star1(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;

	std::getline(X, line);//skip first line as we'll manually make root
	dir root("/");
	dir* currentDir = &root;

	std::vector<std::shared_ptr<dir>> allDirs;
	while (std::getline(X, line))
	{
		if (line == "")continue;

		if (line[0] == '$') {
			if (line[2] == 'c') {// can only be change dir command
				if (line[5] == '.') {// can only be cd.. command
					currentDir = currentDir->parentDir;
				}
				else {
					currentDir = currentDir->getdir(line.substr(5)).get();
					if (!currentDir) {
						return "ERROR FINDING DIR";
					}
				}
			}
			//else, can only be list command which doesnt need action
		}
		else {//can always assume if not $ it is a file from ls command
			if (line[0] == 'd') {// can only be a dir

				std::shared_ptr<dir> newDir = std::make_shared<dir>(line.substr(4));
				currentDir->adddir(newDir);
				allDirs.push_back(newDir);

				//This is where things go wrong, i hoped to also keep pointers to all dirs in a vector seperately
				//allDirs.push_back(std::make_unique<dir>(currentDir->dirs[currentDir->dirs.size()-1]));

			}
			else {
				std::string sizeString, name;
				std::stringstream fs(line);
				fs >> sizeString;
				fs >> name;
				file newFile(name, std::stoi(sizeString));
				currentDir->addfile(newFile);
			}
		}
	}

	int answer = 0;

	for (int i = 0; i < allDirs.size(); i++)
	{
		if (allDirs[i]->childSize < 100000) {
			answer += allDirs[i]->childSize;
		}
	}


	return std::to_string(answer);
}

std::string Day7::Star2(std::string& input, bool example) {
	std::stringstream X(input);
	std::string line;

	std::getline(X, line);//skip first line as we'll manually make root
	dir root("/");
	dir* currentDir = &root;

	std::vector<std::shared_ptr<dir>> allDirs;
	while (std::getline(X, line))
	{
		if (line == "")continue;

		if (line[0] == '$') {
			if (line[2] == 'c') {// can only be change dir command
				if (line[5] == '.') {// can only be cd.. command
					currentDir = currentDir->parentDir;
				}
				else {
					currentDir = currentDir->getdir(line.substr(5)).get();
					if (!currentDir) {
						return "ERROR FINDING DIR";
					}
				}
			}
			//else, can only be list command which doesnt need action
		}
		else {//can always assume if not $ it is a file from ls command
			if (line[0] == 'd') {// can only be a dir

				std::shared_ptr<dir> newDir = std::make_shared<dir>(line.substr(4));
				currentDir->adddir(newDir);
				allDirs.push_back(newDir);

				//This is where things go wrong, i hoped to also keep pointers to all dirs in a vector seperately
				//allDirs.push_back(std::make_unique<dir>(currentDir->dirs[currentDir->dirs.size()-1]));

			}
			else {
				std::string sizeString, name;
				std::stringstream fs(line);
				fs >> sizeString;
				fs >> name;
				file newFile(name, std::stoi(sizeString));
				currentDir->addfile(newFile);
			}
		}
	}

	int neededSpace = 30000000 - (70000000 - root.childSize);
	int smallest = -1;

	for (int i = 0; i < allDirs.size(); i++)
	{
		if (allDirs[i]->childSize > neededSpace) {
			if (smallest == -1 || allDirs[i]->childSize < allDirs[smallest]->childSize) {
				smallest = i;
			}
		}
	}


	return std::to_string(allDirs[smallest]->childSize);
}


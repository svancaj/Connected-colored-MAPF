#pragma once

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <unistd.h> // opts
#include <vector>
#include <list>
#include <algorithm>
#include <iomanip>

#include "max_flow.h"

struct Ed
{
	int from;
	int to;
	int length;

	Ed (int a, int b, int c) : from(a), to(b), length(c) {}

	bool operator < (const Ed& rhs) const
	{
		return length < rhs.length;
	}
};

class Generator
{
public:
	Generator(int, int);
	int ParseMap(std::string);
	void GenerateInstance(int);

private:
	int teams;
	int agents_per_team;
	bool map_parsed;
	int nodes;
	std::string map_name;

	std::vector<std::vector<int> > int_graph;
	std::vector<std::vector<int> > graph;

	std::vector<std::vector<int> > start;
	std::vector<std::vector<int> > goal;


	bool Contains(std::vector<std::vector<int> >&,int);
	std::vector<int> FindNeighbors(int, int, std::vector<std::vector<int> >&);
	int ComputeLB();
	std::vector<Ed> GetDistance(int, std::vector<int>&);
};


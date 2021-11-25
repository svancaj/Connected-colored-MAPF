#include "generator.h"

using namespace std;

Generator::Generator(int t, int apt)
{
	teams = t;
	agents_per_team = apt;
}

void Generator::GenerateInstance(int inst_nr)
{

	if (!map_parsed)
	{
		cout << "Map is not parsed!" << endl;
		return;
	}

	start = vector<vector<int> > (teams, vector<int>());
	goal = vector<vector<int> > (teams, vector<int>());

	for (int t = 0; t < teams; t++)
	{
		bool placed = false;
		while (!placed)
		{
			int val = rand() % nodes;

			if (Contains(start, val))
				continue;

			vector<int> v = FindNeighbors(val, agents_per_team, start);

			if (v.size() == agents_per_team)
			{
				placed = true;
				start[t] = v;
			}
		}

		placed = false;
		while (!placed)
		{
			int val = rand() % nodes;

			if (Contains(goal, val))
				continue;

			vector<int> v = FindNeighbors(val, agents_per_team, goal);

			if (v.size() == agents_per_team)
			{
				placed = true;
				goal[t] = v;
			}
		}
	}

	// PRINT

	stringstream ss;
	ss << "instances/" << map_name << "_" << setfill('0') << setw(2) << teams << "_" << setfill('0') << setw(2) << agents_per_team << "_" << inst_nr << ".pi";
	string filename = ss.str();

	ofstream picat;
	picat.open(filename);
	if (!picat.is_open())
		cout << "Can not open output file " << filename << endl;

	picat << "ins(Graph, As, Groups, LB) =>" << endl;

	// lower bound
	picat << "    LB = " << ComputeLB() << "," << endl;

	// graph
	picat << "    Graph = [" << endl;
	for (int  i = 0; i < nodes; i++)
	{
		if (i != 0)
			picat << "," << endl;
		picat << "    $neibs(" << i + 1 << ",[" << i + 1;

		for (size_t j = 0; j < graph[i].size(); j++)
			picat << "," << graph[i][j] + 1;

		picat << "])";
	}
	picat << endl << "    ]," << endl;

	// agents
	picat << "    As = [";
	for (int i = 0; i < teams; i++)
	{	
		for (int j = 0; j < agents_per_team; j++)
		{
			if (i != 0 || j != 0)
				picat << ",";
			picat << "(" << start[i][j] + 1 << ",[";

			for (int k = 0; k < agents_per_team; k++)
			{
				if (k != 0)
					picat << ",";
				picat << goal[i][k] + 1;
			}
			picat << "])";
		}
	}
	picat << "]," << endl;

	// groups
	picat << "    Groups = [[";
	for (int i = 0; i < teams; i++)
	{
		if (i != 0)
			picat << ",[";
		for (int j = 0; j < agents_per_team; j++)
		{
			if (j != 0)
				picat << ",";
			picat << agents_per_team * i + j + 1;
		}
		picat << "]";
	}
	picat << "]." << endl;

	picat.close();

	cout << "instance " << filename << " printed" << endl;

}

int Generator::ParseMap(string filename)
{
	map_name = filename;
	ifstream in;
	in.open(string("maps/").append(filename));
	if (!in.is_open())
	{
		cout << "Can not open " << filename << endl;
		return -1;
	}

	int rows, columns;
	char c_dump;
	string s_dump;
	getline(in, s_dump); // first line - type

	in >> s_dump >> rows;
	in >> s_dump >> columns;
	in >> s_dump; // map

	// int_graph
	int_graph = vector<vector<int> >(rows, vector<int>(columns, -1));
	nodes = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			in >> c_dump;
			if (c_dump == '.')
			{
				int_graph[i][j] = nodes;
				nodes++;
			}
		}
	}

	// graph
	graph = vector<vector<int> >(nodes, vector<int>());

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (int_graph[i][j] != -1)
			{
				// down
				if (i < rows - 1 && int_graph[i + 1][j] != -1)
				{
					graph[int_graph[i][j]].push_back(int_graph[i + 1][j]);
					graph[int_graph[i + 1][j]].push_back(int_graph[i][j]);
				}

				// left
				if (j > 0 && int_graph[i][j - 1] != -1)
				{
					graph[int_graph[i][j]].push_back(int_graph[i][j - 1]);
					graph[int_graph[i][j - 1]].push_back(int_graph[i][j]);
				}

				// down + left
				/*if (j > 0 && i < rows - 1 && int_graph[i + 1][j - 1] != -1)
				{
					graph[int_graph[i][j]].push_back(int_graph[i + 1][j - 1]);
					graph[int_graph[i + 1][j - 1]].push_back(int_graph[i][j]);
				}*/

				// down + right
				/*if (j < columns - 1 && i < rows - 1 && int_graph[i + 1][j + 1] != -1)
				{
					graph[int_graph[i][j]].push_back(int_graph[i + 1][j + 1]);
					graph[int_graph[i + 1][j + 1]].push_back(int_graph[i][j]);
				}*/
			}
		}
	}

	in.close();

	map_parsed = true;

	cout << "map " << map_name << " parsed\n";

	return nodes;
}


vector<int> Generator::FindNeighbors(int place, int total, vector<vector<int> >& avoid)
{
	vector<int> positions;
	list<int> queue;
	queue.push_back(place);

	while (!queue.empty())
	{
		int currVertex = queue.front();
		queue.pop_front();
		positions.push_back(currVertex);

		if (positions.size() == total)
			break;

		for (size_t i = 0; i < graph[currVertex].size(); i++)
		{
			int adjVertex = graph[currVertex][i];

			if (Contains(avoid, adjVertex) || 
				(find(positions.begin(), positions.end(), adjVertex) != positions.end()) || 
				(find(queue.begin(), queue.end(), adjVertex) != queue.end()))
				continue;

			queue.push_back(adjVertex);
		}
	}
	return positions;
}


bool Generator::Contains(vector<vector<int> >& v, int a)
{
	for (size_t i = 0; i < v.size(); i++)
		if (find(v[i].begin(), v[i].end(), a) != v[i].end())
			return true;

	return false;
}


int Generator::ComputeLB()
{
	int LB = 0;

	for (size_t i = 0; i < start.size(); i++)
	{
		int total_nodes = start[i].size() + goal[i].size() + 2;
		Network g(total_nodes);

		vector<Ed> edges;

		for (size_t j = 0; j < start[i].size(); j++)
		{
			g.addEdge(total_nodes - 2, j, 1);
			g.addEdge(start[i].size() + j, total_nodes - 1, 1);

			vector<Ed> b = GetDistance(start[i][j], goal[i]);
			edges.insert(edges.end(), b.begin(), b.end());

			for (size_t k = 0; k < goal[i].size(); k++)
				g.addEdge(j, start[i].size() + k, 1);
		}

		int last_removed;
		int edge_to_remove = edges.size() - 1;
		sort(edges.begin(), edges.end());

		int flow = g.DinicMaxflow(total_nodes - 2, total_nodes - 1);

		while (flow == start[i].size())
		{
			vector<int>::iterator it = find(start[i].begin(), start[i].end(), edges[edge_to_remove].from);
			int from = distance(start[i].begin(), it);

			it = find(goal[i].begin(), goal[i].end(), edges[edge_to_remove].to);
			int to = start[i].size() + distance(goal[i].begin(), it);

			g.removeEdge(from, to);
			last_removed = edges[edge_to_remove].length;
			edge_to_remove -= 1; 

			flow = g.DinicMaxflow(total_nodes - 2, total_nodes - 1);
		}

		LB = max(LB, last_removed);
	}
	
	return LB;
}


vector<Ed> Generator::GetDistance(int start, vector<int>& goals)
{
	vector<int> distance(graph.size(),graph.size()+1);
	vector<Ed> goal_distance;
	list<int> queue;
	queue.push_back(start);
	distance[start] = 0;
	int goals_found = 0;

	while (!queue.empty())
	{
		int currVertex = queue.front();
		queue.pop_front();

		if (find(goals.begin(), goals.end(), currVertex) != goals.end())
		{
			goal_distance.push_back(Ed(start, currVertex, distance[currVertex]));
			goals_found++;
		}

		if (goals_found == goals.size())
		{
			/*cout << "found all goals!" << endl;
			for (size_t i = 0; i < goal_distance.size(); i++)
			{
				cout << "distance from " << goal_distance[i].from << " to " << goal_distance[i].to << " = " << goal_distance[i].length << endl;
			}*/
			return goal_distance;
		}

		for (size_t i = 0; i < graph[currVertex].size(); i++)
		{
			int adjVertex = graph[currVertex][i];

			if (distance[adjVertex] < graph.size())
				continue;

			queue.push_back(adjVertex);
			distance[adjVertex] = distance[currVertex] + 1;
		}
	}
	return goal_distance;
}
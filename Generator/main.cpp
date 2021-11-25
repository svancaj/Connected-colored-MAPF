#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h> // opts

#include "generator.h"

using namespace std;

void PrintHelp(char*);

int main(int argc, char* argv[])
{
	char *tvalue = NULL;
	char *avalue = NULL;
	char *mvalue = NULL;
	char *xvalue = NULL;
	char *svalue = NULL;
	bool hvalue = false;

	// parse arguments
	opterr = 0;
	char c;
	while ((c = getopt (argc, argv, "ht:a:m:x:s:")) != -1)
	{
		switch (c)
		{
			case 't':
				tvalue = optarg;
				break;
			case 'a':
				avalue = optarg;
				break;
			case 'm':
				mvalue = optarg;
				break;
			case 'x':
				xvalue = optarg;
				break;
			case 's':
				svalue = optarg;
				break;
			case 'h':
				hvalue = true;
				break;
			case '?':
				if (optopt == 't' || optopt == 'a' || optopt == 'm' || optopt == 'x')
				{
					cerr << "Option -" << (char)optopt << " requires an argument!" << endl;
					return -1;
				}
				// unknown option - ignore it
				break;
			default:
				return -1; // should not get here;
		}
	}

	// set problem specification
	bool all_good = true;

	int teams;
	int agents_per_team;
	int times = 1;
	int seed;

	// help
	if (hvalue)
	{
		PrintHelp(argv[0]);
		return 0;
	}

	// number of teams
	if (tvalue != NULL)
		teams = atoi(tvalue);
	else
		all_good = false;

	// agents per team
	if (avalue != NULL)
		agents_per_team = atoi(avalue);
	else
		all_good = false;

	// map name
	if (mvalue == NULL)
		all_good = false;

	// times
	if (xvalue != NULL)
		times = atoi(xvalue);

	// seed
	if (svalue != NULL)
		seed = atoi(svalue);
	else
		all_good = false;

	if (!all_good)
	{
		cerr << "Some argumets are missing!" << endl;
		PrintHelp(argv[0]);
		return -1;
	}

	srand (seed);

	Generator gen(teams, agents_per_team);

	if (gen.ParseMap(mvalue) == -1)
		return -1;

	for (int i = 0; i < times; i++)
		gen.GenerateInstance(i);

	return 0;
}

void PrintHelp(char* program_name)
{
	cout << "Help message for program " << program_name << endl;
	return;
}

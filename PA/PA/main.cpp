#include <iostream>
#include <vector>
#include "DungeonLevel.h"
using namespace std;

/* UNIT TEST
	Will be moved into its own file so that it can be made as a seperate executable. 
	Returns 0 if all tests pass, else returns -1.*/
bool unitTest(DungeonLevel dl)
{
	bool passed = true;

	//Get the vector of floors from the dungeon level. Do it by value to ensure we don't change the value
	vector<Floor *> vFloors = dl.getFloors();

	//Now we will loop through the floors, for each one we will test for all requirements	
	for (int floor = 0; floor < vFloors.size(); floor++)
	{
		cout << endl << "Floor: " << floor << endl;
		vector<vector<Tile *>> map = vFloors[floor]->getMap();

		//The first check is to see if the map is the correct size
		cout << "Dungeon Size Test" << endl;
		
		//Must check each row of the map vector and see if it has 79 columns
		bool widthCheck = true;
		int failCond = -1;
		for (int row = 0; row < map.size(); row++)
		{
			if (map[row].size() != 79) {widthCheck = false; failCond = map[row].size();}
		}

		if (map.size() != 20 || !widthCheck)
		{
			cout << "Failed Dungeon Size Test: Dungeon is " << map.size() << " high and " << failCond << "wide!" << endl;
			passed = false;
		}
		else
		{
			cout << "Passed Dungeon Size Test!" << endl;
		}
	}

	return passed;
}


int main()
{
	DungeonLevel test(2);
	test.printDungeon(cout);

	unitTest(test);

	cin.ignore(200, '\n');
	return 0;
}
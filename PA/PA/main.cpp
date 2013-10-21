#include <iostream>
#include <vector>
#include "DungeonLevel.h"
#include "Tile.h"
#include "Floor.h"
using namespace std;

//Helper function in unit test to reset the map of the dungeon
void resetMap(vector<vector<Tile *>> & vMap)
{
	for (int row = 0; row < vMap.size(); row++)
	{
		for (int col = 0; col < vMap[row].size(); col++)
		{
			if (vMap[row][col]->getSymbol() == 'X') {vMap[row][col]->setSymbol('.');}
			else if (vMap[row][col]->getSymbol() == 'O') {vMap[row][col]->setSymbol('#');}
			else if (vMap[row][col]->getSymbol() == '^') {vMap[row][col]->setSymbol('>');}
			else if (vMap[row][col]->getSymbol() == '_') {vMap[row][col]->setSymbol('<');}
		}
	}
}

//Helper fucntion for debugging, prints any vector<vector<Tile *>> I give it to the console.
void printMap(vector<vector<Tile *>> & vMap)
{
	for (int row = 0; row < vMap.size(); row++)
	{
		for (int col = 0; col < vMap[row].size(); col++)
		{
			vMap[row][col]->printTile(cout);
		}
		cout << endl;
	}
}

//Helper function to check that all rooms are connected. Takes in the map we are testing by reference and the location of the first room tile to check
//Marks rooms with 'X', tunnels with 'O', upstairs with '^', and downstairs with '_' so that we can reset the map if necessary.
void recurseMark(vector<vector<Tile *>> & vMap, int row, int col)
{
	//Ending condition
	if (vMap[row][col]->getSymbol() == ' ' || vMap[row][col]->getSymbol() == 'X' || vMap[row][col]->getSymbol() == 'O' || vMap[row][col]->getSymbol() == '^' || vMap[row][col]->getSymbol() == '_') {return;}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol('X');}
	else if (vMap[row][col]->getSymbol() == '#') {vMap[row][col]->setSymbol('O');}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol('_');}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol('^');}

	//Now call this function on all adjacent tiles
	//Use a counter so we can control the break and protect against out of bounds indices access
	int outCounter = 0;
	int j = row - 1;
	while (outCounter <= 3)
	{
		//Need to make sure we don't check out of bounds indices
		//For now, to avoid global variable and to amke it easy we will use hard limits on the size and height instead of passing
		//the requirements (because we will use only one size for this assignment.
		if (j < 0) {j = 0; outCounter++;}
		else if (j >= 20) {j = 20 - 1; outCounter++;}

		//Use a counter so we can control the break and protect against out of bounds indices access
		int inCounter = 0;
		int i = col - 1;
		while (inCounter <= 3)
		{
			//Need to make sure we don't check out of bounds indices
			if (i < 0) {i = 0; inCounter++;}
			else if (i >= 79) {i = 79 - 1; inCounter++;}

			//Call the function recursively
			recurseMark(vMap, j, i);

			i++;
			inCounter++;
		}

		j++;
		outCounter++;
	}
}

//Modified recurseMark that only marks the room tiles and stairs and counts while it does it. Then returns the integer, representing the number of tiles in the room
void recurseCount(vector<vector<Tile *>> & vMap, int row, int col, int & count)
{
	//Ending condition
	if (vMap[row][col]->getSymbol() == ' ' || vMap[row][col]->getSymbol() == 'X' || vMap[row][col]->getSymbol() == '#' || vMap[row][col]->getSymbol() == '^' || vMap[row][col]->getSymbol() == '_') {return;}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol('X'); count++;}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol('_'); count++;}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol('^'); count++;}

	//Now call this function on all adjacent tiles
	//Use a counter so we can control the break and protect against out of bounds indices access
	int outCounter = 0;
	int j = row - 1;
	while (outCounter <= 3)
	{
		//Need to make sure we don't check out of bounds indices
		//For now, to avoid global variable and to amke it easy we will use hard limits on the size and height instead of passing
		//the requirements (because we will use only one size for this assignment.
		if (j < 0) {j = 0; outCounter++;}
		else if (j >= 20) {j = 20 - 1; outCounter++;}

		//Use a counter so we can control the break and protect against out of bounds indices access
		int inCounter = 0;
		int i = col - 1;
		while (inCounter <= 3)
		{
			//Need to make sure we don't check out of bounds indices
			if (i < 0) {i = 0; inCounter++;}
			else if (i >= 79) {i = 79 - 1; inCounter++;}

			//Call the function recursively
			if (j != row && i != col) {recurseCount(vMap, j, i, count);}

			i++;
			inCounter++;
		}

		j++;
		outCounter++;
	}
}

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

		//DUNGEON SIZE TEST
		cout << "Dungeon Size Test:" << endl;
		
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
			cout << "Dungeon Size Test Passed!" << endl;
		}

		//DUNGEON AREA TEST
		cout << "Dungeon Area Test:" << endl;

		//Loop over all the tiles and count the number of room tiles
		int count = 0;
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() == '.') {count++;}
			}
		}

		if (count < 200)
		{
			passed = false;
			cout << "Dungeon Area Test Failed: The number of room tiles in the dungeon is " << count << "!" << endl;
		}
		else {cout << "Dungeon Area Test Passed!" << endl;}

		//RECTANGULAR AND OVERLAP TEST


		//UPSTAIRS AND DOWNSTAIRS TEST
		cout << "Upstairs and Downstairs Test:" << endl;

		//Loop over the vector and see if an upstairs and downstairs tile exist
		bool up = false;
		bool down = false;
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() == '<') {down = true;}
				else if (map[row][col]->getSymbol() == '>') {up = true;}
			}
		}

		if (!up || !down)
		{
			passed = false;
			cout << "Upstairs and Downstairs Test Failed: Downstairs is " << down << " and Upstairs is " << up << endl;
		}
		else {cout << "Upstairs and Downstairs Test Passed!" << endl;}

		//PASSABILITY TEST
		//Will use a recursive helper function to recursively go through all touching tiles
		//First iterate through and find a room tile
		cout << "Room Passability Test:" << endl;

		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() == '.') {recurseMark(map, row, col);}
			}
		}

		//Now check if there are any room tiles left, if so we fail
		bool passTest = true;
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() == '.') {passTest = false;}
			}
		}

		if (!passTest) {passed = false; cout << "Room Passability Test Failed: There is a passable tile left!" << endl;}
		else {cout << "Room Passability Test Passed!" << endl;}

		//ROOM MINIMUM SIZE TEST
		//Will iterate over a map until we find a room tile. Then we Will use a recursive function to count each
		//room tile and mark them. Then the function will continue to look for room tiles and do the same for each room.
		//It'll return a count of the room with the fewest tiles
		resetMap(map);

		bool passSize = true;
		cout << "Room Size Test:" << endl;

		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				int iCount = 0;
				if (map[row][col]->getSymbol() == '.')
				{
					recurseCount(map, row, col, iCount);
					printMap(map);
					cout << iCount << endl;
					if (iCount < 16) {passSize = false;}
				}
			}
		}

		if (!passSize) {passed = false; cout << "Room Size Test Failed: There is less than 16 tiles in at least one room!" << endl;}
		else {cout << "Room Size Test Passed!" << endl;}

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
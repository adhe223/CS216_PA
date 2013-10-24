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
	//Check if oob
	if (row < 0 || col < 0 || row >= vMap.size() || col >= vMap[0].size()) {return;}

	//Check if already marked or space we don't want to mark
	if (vMap[row][col]->getSymbol() == ' ' || vMap[row][col]->getSymbol() == 'X' || vMap[row][col]->getSymbol() == 'O' || vMap[row][col]->getSymbol() == '^' || vMap[row][col]->getSymbol() == '_') {return;}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol('X');}
	else if (vMap[row][col]->getSymbol() == '#') {vMap[row][col]->setSymbol('O');}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol('_');}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol('^');}

	//Now call this function on all adjacent tiles
	recurseMark(vMap, row - 1, col);
	recurseMark(vMap, row + 1, col);
	recurseMark(vMap, row, col - 1);
	recurseMark(vMap, row, col + 1);
}

//Modified recurseMark that only marks the room tiles and stairs and counts while it does it. Count is the number of tiles in the room
void recurseCount(vector<vector<Tile *>> & vMap, int row, int col, int & count)
{
	//Ending condition
	//Check if oob
	if (row < 0 || col < 0 || row >= vMap.size() || col >= vMap[0].size()) {return;}

	//Check if already marked or space we don't want to mark
	if (vMap[row][col]->getSymbol() == ' ' || vMap[row][col]->getSymbol() == 'X' || vMap[row][col]->getSymbol() == '#' || vMap[row][col]->getSymbol() == '^' || vMap[row][col]->getSymbol() == '_') {return;}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol('X'); count++;}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol('_'); count++;}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol('^'); count++;}

	//Now call this function on all adjacent tiles
	recurseCount(vMap, row - 1, col, count);
	recurseCount(vMap, row + 1, col, count);
	recurseCount(vMap, row, col - 1, count);
	recurseCount(vMap, row, col + 1, count);
}

void overlapRecurse(vector<vector<Tile *>> & vMap, int row, int col, int & count, int & roomWidth, int & roomHeight)
{
	//Ending condition
	//Check if oob
	if (row < 0 || col < 0 || row >= vMap.size() || col >= vMap[0].size()) {return;}

	//Check if already marked or space we don't want to mark
	char sym = vMap[row][col]->getSymbol();
	if (sym == ' ' || sym == '#' || sym == '^' || sym == '_' || sym == 'X') {return;}

	//Initial check on how wide and tall the room is (used for rectangular test)
	if (roomWidth == 0)
	{
		int newCol = col;
		int newRow = row;
		while (vMap[row][newCol]->getSymbol() == '.' || vMap[row][newCol]->getSymbol() == '<' || vMap[row][newCol]->getSymbol() == '>')
		{
			roomWidth++;
			newCol++;
		}

		while (vMap[newRow][col]->getSymbol() == '.' || vMap[newRow][col]->getSymbol() == '<' || vMap[newRow][col]->getSymbol() == '>')
		{
			roomHeight++;
			newRow++;
		}
	}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol('X'); count++;}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol('_'); count++;}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol('^'); count++;}

	overlapRecurse(vMap, row - 1, col, count, roomWidth, roomHeight);
	overlapRecurse(vMap, row + 1, col, count, roomWidth, roomHeight);
	overlapRecurse(vMap, row, col - 1, count, roomWidth, roomHeight);
	overlapRecurse(vMap, row, col + 1, count, roomWidth, roomHeight);
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

		printMap(map);

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

		int startRow = -1;
		int startCol = -1;

		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() == '.' || map[row][col]->getSymbol() == '#')
				{
					if (startRow == -1) {startRow = row; startCol = col;}
				}
			}
		}
		recurseMark(map, startRow, startCol);

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
					if (iCount < 16) {passSize = false;}
				}
			}
		}

		if (!passSize) {passed = false; cout << "Room Size Test Failed: There is less than 16 tiles in at least one room!" << endl;}
		else {cout << "Room Size Test Passed!" << endl;}

		//OVERLAP TEST AND RECTANGULAR TEST
		/*The overlap test is predicated on there being 6 rooms in the dungeon. We assume that if there are less rooms then there must be an overlap.
			We use a recursive algorithm that marks the rooms only (not the tunnels) and a simple count to see how many rooms are in the dungeon.
			
			For the rectangular test we find the height and width of the room (from the top left corner of the room). We also calculate a count from the
			room using our recursive marking algorithm. We then check that the count divided by the width is equal to the height that we have found. If it
			doesn't, then the room is not a rectangle.*/
		resetMap(map);

		cout << "Room Overlap Test:" << endl;

		int numRooms = 0;
		bool isRectangular = true;
		bool overlapPass = true;
		
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				int iCount = 0;
				int roomWidth = 0;
				int roomHeight = 0;
				if (map[row][col]->getSymbol() == '.' || map[row][col]->getSymbol() == '<' || map[row][col]->getSymbol() == '>')
				{
					overlapRecurse(map, row, col, iCount, roomWidth, roomHeight);
					numRooms++;

					//Now check if room is rectangular
					double dCalc = (iCount / roomWidth);
					double dHeight = roomHeight;
					if (dCalc != dHeight)
					{
						isRectangular = false;
					}
				}
			}
		}

		if (numRooms < 6) {overlapPass = false;}

		if (!overlapPass) {passed = false; cout << "Room Overlap Test Failed: One or more rooms overlap each other!" << endl;}
		else {cout << "Room Overlap Test Passed!" << endl;}

		cout << "Rectangular Room Test:" << endl;
		if (!isRectangular) {passed = false; cout << "Rectangular Room Test Failed: One or more rooms is not rectangular!" << endl;}
		else {cout << "Rectangular Room Test Passed!" << endl;}
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
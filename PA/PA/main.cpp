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

void overlapRecurse(vector<vector<Tile *>> & vMap, int row, int col, int & count, int & vIndex, vector<char> & symbols, int & roomWidth, int & roomHeight)
{
	//Initial check on how wide and tall the room is (used for rectangular test)
	if (roomWidth == 0)
	{
		int newCol = col;
		int newRow = row;
		while (vMap[row][newCol]->getSymbol() == '.')
		{
			roomWidth++;
			newCol++;
		}

		while (vMap[newRow][col]->getSymbol() == '.')
		{
			roomHeight++;
			newRow++;
		}
	}

	//Ending condition
	char sym = vMap[row][col]->getSymbol();
	if (sym == ' ' || sym == '#' || sym == '^' || sym == '_' || sym == symbols[vIndex]) {return;}

	//Mark the tile
	if (vMap[row][col]->getSymbol() == '.') {vMap[row][col]->setSymbol(symbols[vIndex]); count++;}
	else if (vMap[row][col]->getSymbol() == '<') {vMap[row][col]->setSymbol(symbols[vIndex]); count++;}
	else if (vMap[row][col]->getSymbol() == '>') {vMap[row][col]->setSymbol(symbols[vIndex]); count++;}
	//To check for overlaps, we must now have all possible room marks as tiles we mark (so the marking will overlap)
	else
	{
		for (int i = 0; i < symbols.size(); i++)
		{
			if (vMap[row][col]->getSymbol() == symbols[i])
			{
				vMap[row][col]->setSymbol(symbols[vIndex]);
				count++;
			}
		}
	}


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
			if (j != row && i != col) {overlapRecurse(vMap, j, i, count, vIndex, symbols, roomWidth, roomHeight);}

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
					if (iCount < 16) {passSize = false;}
				}
			}
		}

		if (!passSize) {passed = false; cout << "Room Size Test Failed: There is less than 16 tiles in at least one room!" << endl;}
		else {cout << "Room Size Test Passed!" << endl;}

		//OVERLAP TEST AND RECTANGULAR TEST
		/*This is the most complicated of all the unit tests. The idea is similar to the count algorithm I used in the room size test.
		The idea is that I will mark a room with a symbol, then use a vector to keep count of the symbol. We will then switch
		swymbols for the next room to be marked and mark it and count. (will use a vector of size 6 with different symbols and simply iterate
		through that). Then after all rooms have been marked with their respective symbols, we will iterate through all tiles and use a new
		vector to keep track of counts. Each time we see a symbol we increment the corresponding count
		of that symbol (similar to building a histogram for counting sort). Finally we can check the counts of the symbolds in the vector we know is
		correct to  those of the new vector. If the rooms don't overlap the counts will be the same.*/
		resetMap(map);

		cout << "Room Overlap Test:" << endl;

		//Initialize the necessary containers
		vector<char> symbols;
		symbols.push_back('!');
		symbols.push_back('@');
		symbols.push_back('$');
		symbols.push_back('%');
		symbols.push_back('&');
		symbols.push_back('*');
		vector<int> vCount;
		vCount.push_back(0);
		vCount.push_back(0);
		vCount.push_back(0);
		vCount.push_back(0);
		vCount.push_back(0);
		vCount.push_back(0);
		vector<int> vNewCount;
		vNewCount.push_back(0);
		vNewCount.push_back(0);
		vNewCount.push_back(0);
		vNewCount.push_back(0);
		vNewCount.push_back(0);
		vNewCount.push_back(0);
		int vIndex = 0;
		bool isRectangular = true;
		
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				int iCount = 0;
				int roomWidth = 0;
				int roomHeight = 0;
				if (map[row][col]->getSymbol() == '.' || map[row][col]->getSymbol() == '<' || map[row][col]->getSymbol() == '>')
				{
					overlapRecurse(map, row, col, iCount, vIndex, symbols, roomWidth, roomHeight);
					vCount[vIndex] = iCount;
					vIndex++;

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

		//Now we must loop over all tiles and get the new count
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[row].size(); col++)
			{
				if (map[row][col]->getSymbol() != ' ' && map[row][col]->getSymbol() != '#')
				{
					//Increment count for correct symbols
					char sym = map[row][col]->getSymbol();
					if (sym == '!') {vNewCount[0] = vNewCount[0] + 1;}
					else if (sym == '@') {vNewCount[1] = vNewCount[1] + 1;}
					else if (sym == '$') {vNewCount[2] = vNewCount[2] + 1;}
					else if (sym == '%') {vNewCount[3] = vNewCount[3] + 1;}
					else if (sym == '&') {vNewCount[4] = vNewCount[4] + 1;}
					else if (sym == '*') {vNewCount[5] = vNewCount[5] + 1;}
				}
			}
		}

		//Now check if the new counts match
		bool overlapPass = true;
		for (int i = 0; i < symbols.size(); i++)
		{
			if (vCount[i] != vNewCount[i])
			{
				overlapPass = false;
			}
		}

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
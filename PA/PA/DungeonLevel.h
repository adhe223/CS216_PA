#ifndef _DungeonLevel_included_
#define _DungeonLevel_included_

#include <vector>
#include "Tile.h"
#include "Floor.h"

class DungeonLevel
{
public:
	DungeonLevel(int numFloors);		//Constructs map
	
	void printDungeon(std::ostream & output);

	std::vector<Floor *> getFloors() const;

private:
	int iNumFloors;	//Number of floors in dungeon
	std::vector<Floor *> vFloors;	//Vector of floors in dungeon
	int iWidth;		//width of dungeon. We will not make getter setter because we want it to always be 79 for now
	int iHeight;	//height of dungeon, always 20 for now
};

#endif
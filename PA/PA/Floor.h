#ifndef _Floor_included_
#define _Floor_included_

#include <vector>
#include "Tile.h"

class Floor
{
public:
	Floor(int floor, int numFloors, int floorWidth, int floorHeight);		//Constructs floor
	
	int getFloor() const;
	std::vector<std::vector<Tile *>> getMap() const;

	void printFloor(std::ostream & output);

private:
	void drawBlank();	//Helper function that fills vector with blank spaces
	void drawTunnel();	//Helper function that adds the tunnel to the vector
	void drawRooms();	//Helper function that adds the rooms to the vector
	void drawStairs();	//Helper function that adds the stairs into the vector

	int iFloor;		//Which floor this is
	int iNumFloors;	//Number of floors in dungeon (need to draw stairs)
	std::vector<std::vector<Tile *>> vMap;	//2D vector of tile objects that is our map
	int iWidth;		//width of floor. We will not make getter setter because we want it to always be 79 for now
	int iHeight;	//height of floor, always 20 for now

	int iSectorWide;	//Number of sectors our dungeon has widthwise
	int iSectorHigh;	//Number of sectors our dungeon has heightwise
	int iSectorHeight;	//Height of sectors
	int iSectorWidth;	//Width of sectors

	int iMinRoomSide;	//Smallest square room
	int iMaxRoomWidth;	//Largest width a room can have
	int iMaxRoomHeight;	//Largest height a room can have

	
};

#endif
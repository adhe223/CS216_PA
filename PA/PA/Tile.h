#ifndef _Tile_included_
#define _Tile_included_

#include <ostream>

class Tile
{
public:
	Tile();
	Tile(char symbol, bool pass);

	char getSymbol() const;
	void setSymbol(char symbol);
	bool isPassable() const;
	void setPassable(bool pass);

	void printTile(std::ostream & output);

private:
	char cSymbol;
	bool bPassable;
};

#endif
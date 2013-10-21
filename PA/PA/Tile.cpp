#include "Tile.h"
using namespace std;

Tile::Tile()
{
	//default value is impassable tile
	cSymbol = ' ';
	bPassable = false;
}

Tile::Tile(char symbol, bool pass)
{
	cSymbol = symbol;
	bPassable = pass;
}

char Tile::getSymbol() const
{
	return cSymbol;
}

void Tile::setSymbol(char symbol)
{
	cSymbol = symbol;
}

bool Tile::isPassable() const
{
	return bPassable;
}

void Tile::setPassable(bool pass)
{
	bPassable = pass;
}

void Tile::printTile(std::ostream & output)
{
	output << getSymbol();
}
#include "Tile.h"
using namespace std;

Tile::Tile() {}

Tile::Tile(bool isMine)
{
	this->isMine = isMine;
}

bool Tile::Mine()
{
	return isMine;
}

bool Tile::Hidden()
{
	return isHidden;
}

bool Tile::Flagged()
{
	return isFlagged;
}

bool Tile::IsClicked()
{
	return isClicked;
}

unsigned int Tile::NumAdjacentMines()
{
	return numAdjacentMines;
}

vector<Tile*> Tile::Adjacents()
{
	return adjacentTiles;
}

void Tile::ChangeMine()
{
	if (isMine == true)
		isMine = false;
	else
		isMine = true;
}

void Tile::Unhide()
{
	isHidden = false;
}

void Tile::ClickTile()
{
	isClicked = true;
}

void Tile::ChangeFlagged()
{
	if (isFlagged == true)
		isFlagged = false;
	else
		isFlagged = true;
}

void Tile::AddAdjacentMine() 
{
	if (numAdjacentMines < 8)
		numAdjacentMines++;
}

void Tile::AddAdjacentTile(Tile* tile)
{
	if (adjacentTiles.size() < 8)
		adjacentTiles.push_back(tile);
}

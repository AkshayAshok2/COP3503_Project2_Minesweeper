#pragma once
#include <vector>
using namespace std;

class Tile
{
	bool isMine;
	bool isHidden = true;
	bool isFlagged = false;
	bool isClicked = false;
	unsigned int numAdjacentMines = 0;
	vector<Tile*> adjacentTiles;

public:
	Tile();
	Tile(bool isMine);

	bool Mine();
	bool Hidden();
	bool Flagged();
	bool IsClicked();

	unsigned int NumAdjacentMines();
	vector<Tile*> Adjacents();

	void ChangeMine();
	void Unhide();
	void ClickTile();
	void ChangeFlagged();
	void AddAdjacentMine();
	void AddAdjacentTile(Tile* tile);
};
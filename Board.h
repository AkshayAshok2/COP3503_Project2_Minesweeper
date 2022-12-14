#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include "TextureManager.h"
#include "Tile.h"
using namespace std;

class Board 
{
	unsigned int width;
	unsigned int widthPixels;
	unsigned int height;
	unsigned int heightPixels;
	unsigned int numTiles;
	unsigned int numMines;
	unsigned int numRevealedTiles = 0;
	int mineCounter;
	bool debugMode = false; 
	vector<vector<Tile*>> tiles;

	enum GameState { playing, win, lose };
	GameState gameState = playing;

public:
	Board();
	Board(unsigned int _width, unsigned int _height, unsigned int tileCount, unsigned int mineCount);

	void WhichTile(int& x, int& y, string button = "left");
	void DrawBoard(sf::RenderWindow& window);
	void DrawButtons(sf::RenderWindow& window);
	void GetConfig();
	void FillBoard(string fileName);
	void GetAdjacentTiles();
	void RevealTiles(Tile* tile);
	void Randomize();
	void GameWon();
	
	unsigned int GetWidth();
	unsigned int GetHeight();
	unsigned int GetNumTiles();
	unsigned int GetNumMines();
	GameState GetGameState();
};




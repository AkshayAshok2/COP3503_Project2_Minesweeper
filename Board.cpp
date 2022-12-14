#include "Board.h"
using namespace std;

Board::Board() {}

Board::Board(unsigned int _width, unsigned int _height, unsigned int tileCount, unsigned int mineCount)
{
	width = _width;
	height = _height;
	numTiles = tileCount;
	numMines = mineCount;
}

unsigned int Board::GetWidth()
{
	return this->widthPixels;
}

unsigned int Board::GetHeight()
{
	return this->heightPixels;
}

unsigned int Board::GetNumTiles()
{
	return this->numTiles;
}

unsigned int Board::GetNumMines()
{
	return this->numMines;
}

Board::GameState Board::GetGameState()
{
	return gameState;
}

void Board::GetConfig()
{
    ifstream inFile("boards/config.cfg");

	if (inFile.is_open())
	{
		inFile >> width;
		inFile >> height;
		inFile >> numMines;

		numTiles = width * height;
		widthPixels = width * 32;
		heightPixels = height * 32 + 100;
		mineCounter = numMines;
	}
	else
		cout << "File \"config.cfg\" could not be opened." << endl;
}

void Board::FillBoard(string fileName)
{
	tiles.clear();
	
	numMines = 0;
	numRevealedTiles = 0;
	debugMode = false;
	gameState = playing;

	ifstream inFile(fileName);
	
	if (inFile.is_open())
	{
		string thisRow;
		char token;

		while (getline(inFile, thisRow))
		{
			istringstream thisRowStream(thisRow);
			vector<Tile*> rowTiles;

			while (thisRowStream >> token)
			{
				if (token == '1')
				{
					Tile* newTile = new Tile(true);
					rowTiles.push_back(newTile);
					numMines++;
				}
				else
				{
					Tile* newTile = new Tile(false);
					rowTiles.push_back(newTile);
				}
			}

			mineCounter = numMines;

			if (rowTiles.size() < width)
			{
				unsigned int difference = width - rowTiles.size();

				for (unsigned int x = 0; x < difference; x++)
				{
					Tile* newTile = new Tile(false);
					rowTiles.push_back(newTile);
				}
			}

			tiles.push_back(rowTiles);
		}

		if (tiles.size() < height)
		{
			unsigned int difference = height - tiles.size();

			for (unsigned int y = 0; y < difference; y++)
			{
				vector<Tile*> rowTiles;

				for (unsigned int x = 0; x < width; x++)
				{
					Tile* newTile = new Tile(false);
					rowTiles.push_back(newTile);
				}

				tiles.push_back(rowTiles);
			}
		}

		GetAdjacentTiles();
	}
	else
	{
		cout << "File " << fileName << " could not be opened correctly." << endl;
	}
}

void Board::GetAdjacentTiles()
{
	for (int y = 0; y < height; y++)
	{ 
		for (int x = 0; x < width; x++)
		{
			for (int yRel = y - 1; yRel < y + 2; yRel++)
			{
				if (yRel < 0 || yRel >= height)
					continue;

				for (int xRel = x - 1; xRel < x + 2; xRel++)
				{
					if (xRel < 0 || xRel >= width || (xRel == x && yRel == y))
						continue;

					if (tiles[yRel][xRel]->Mine())
						tiles[y][x]->AddAdjacentMine();

					tiles[y][x]->AddAdjacentTile(tiles[yRel][xRel]);
				}
			}
		}
	}
}

void Board::RevealTiles(Tile* tile)
{
	tile->Unhide();
	tile->ClickTile();
	numRevealedTiles++;
	
	if (!tile->Mine() && tile->NumAdjacentMines() == 0)
	{
		for (unsigned int i = 0; i < tile->Adjacents().size(); i++)
		{
			auto nextTile = tile->Adjacents().at(i);

			if (!nextTile->Mine() && !nextTile->Flagged() && !nextTile->IsClicked())
				RevealTiles(tile->Adjacents().at(i));
		}
	}
}

void Board::Randomize()
{
	GetConfig();

	if (tiles.size() > 0)
		tiles.clear();

	numRevealedTiles = 0;
	debugMode = false;
	gameState = playing;
	unsigned int minesPlaced = 0;

	// First, create board with no mines
	for (unsigned int y = 0; y < height; y++)
	{
		vector<Tile*> rowTiles;

		for (unsigned int x = 0; x < width; x++)
		{ 
			Tile* newTile = new Tile(false);
			rowTiles.push_back(newTile);
		}

		tiles.push_back(rowTiles);
	}

	random_device rd; // obtain a random number from hardware
	mt19937 gen(rd()); // seed the generator
	uniform_int_distribution<> distr(0, height - 1); // define the range for height
	uniform_int_distribution<> distr2(0, width - 1); // define the range for width

	// While loop to fill in mines randomly until total number of mines is fulfilled
	while (minesPlaced < numMines)
	{
		unsigned int randY = distr(gen); // choose random y val
		unsigned int randX = distr2(gen); // choose random x val

		if (!tiles[randY][randX]->Mine())
		{
			tiles[randY][randX]->ChangeMine();
			minesPlaced++;
		}
	}

	GetAdjacentTiles();
}

void Board::WhichTile(int& x, int& y, string button)
{
	if (y < heightPixels - 100 && gameState == playing)
	{
		x /= 32; y /= 32;

		if (tiles[y][x]->Hidden())
		{
			if (button == "left" && !tiles[y][x]->Flagged())
			{
				RevealTiles(tiles[y][x]);

				if (tiles[y][x]->Mine())
				{
					gameState = lose;
					debugMode = true;
					cout << "You lose!" << endl;
				}
				else if (numRevealedTiles == numTiles - numMines)
				{
					gameState = win;
					GameWon();
					cout << "You win!" << endl;
				}

			}
			else if (button == "right")
			{
				tiles[y][x]->ChangeFlagged();

				if (tiles[y][x]->Flagged())
					mineCounter--;
				else if (!tiles[y][x]->Flagged() && mineCounter < numMines)
					mineCounter++;

				cout << "Number of mines on the board: " << mineCounter << endl;
			}

			cout << "Number of revealed tiles: " << numRevealedTiles << endl;
		}
	}
	else if (y >= heightPixels - 100 && y <= heightPixels - 36)
	{
		// if statement for finding smiley face, debug, and test buttons
		// smiley
		if (x >= widthPixels / 2 - 32 && x <= widthPixels / 2 + 32)
		{
			cout << "Smiley face button pressed." << endl;
			cout << "New game started." << endl;
			Randomize();
		}
		// test 3
		else if (x >= widthPixels - 64 && x < widthPixels)
		{
			cout << "Test 3 pressed." << endl;
			FillBoard("boards/testboard3.brd");
		}
		// test 2
		else if (x >= widthPixels - 128 && x < widthPixels - 64)
		{
			cout << "Test 2 pressed." << endl;
			FillBoard("boards/testboard2.brd");
		}
		// test 1
		else if (x >= widthPixels - 198 && x < widthPixels - 128)
		{
			cout << "Test 1 pressed." << endl;
			FillBoard("boards/testboard1.brd");
		}
		// debug
		else if (x >= widthPixels - 256 && x < widthPixels - 198 && gameState == playing)
		{
			cout << "Debug button pressed." << endl;

			if (debugMode == false)
				debugMode = true;
			else
				debugMode = false;
		}
		else
			cout << "Blank space..." << endl;
	}
}

void Board::DrawBoard(sf::RenderWindow& window)
{
	// Iteratively draw board tiles
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			sf::Sprite square;
			sf::Sprite number;
			sf::Sprite flag;
			sf::Sprite mine;
            square.setPosition(x * 32.f, y * 32.f);
			number.setPosition(x * 32.f, y * 32.f);
			mine.setPosition(x * 32.f, y * 32.f);
			flag.setPosition(x * 32.f, y * 32.f);

			if (tiles[y][x]->Hidden())
			{
				square.setTexture(TextureManager::GetTexture("tile_hidden"));

				if (tiles[y][x]->Flagged())
				{
					flag.setTexture(TextureManager::GetTexture("flag"));
				}

				if (debugMode && tiles[y][x]->Mine())
					mine.setTexture(TextureManager::GetTexture("mine"));
			}
			else
			{
				square.setTexture(TextureManager::GetTexture("tile_revealed"));

				if (tiles[y][x]->Mine())
					mine.setTexture(TextureManager::GetTexture("mine"));
				else
				{
					if (tiles[y][x]->NumAdjacentMines() > 8)
					{
						cout << "x: " << x << endl;
						cout << "y: " << y << endl;
					}
					else if (tiles[y][x]->NumAdjacentMines() > 0)
					{
						string arg = "number_";
						int arg2 = tiles[y][x]->NumAdjacentMines();
						arg += to_string(arg2);

						number.setTexture(TextureManager::GetTexture(arg));
					}
				}
			}

			window.draw(square);
			window.draw(number);

			if (debugMode)
			{
				window.draw(flag);
				window.draw(mine);
			}
			else
			{
				window.draw(mine);
				window.draw(flag);
			}
		}
	}
}

void Board::DrawButtons(sf::RenderWindow& window) 
{
	// Draw command buttons underneath board
	sf::Sprite smiley;
	sf::Sprite debug;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite test3;

	sf::Sprite digit1;
	sf::Sprite digit2;
	sf::Sprite digit3;
	sf::Sprite digit4;

	int yPosition = heightPixels - 100;

	// For loop for buttons
	for (unsigned int x = 0; x < width; x++)
	{
		if (x == width / 2)
		{
			smiley.setPosition(x * 32.f, yPosition);

			if (gameState == playing)
				smiley.setTexture(TextureManager::GetTexture("face_happy"));
			else if (gameState == win)
				smiley.setTexture(TextureManager::GetTexture("face_win"));
			else if (gameState == lose)
				smiley.setTexture(TextureManager::GetTexture("face_lose"));
		}
		else if (x == width - 8)
		{
			debug.setPosition(x * 32.f, yPosition);
			debug.setTexture(TextureManager::GetTexture("debug"));
		}
		else if (x == width - 6)
		{
			test1.setPosition(x * 32.f, yPosition);
			test1.setTexture(TextureManager::GetTexture("test_1"));
		}
		else if (x == width - 4)
		{
			test2.setPosition(x * 32.f, yPosition);
			test2.setTexture(TextureManager::GetTexture("test_2"));
		}
		else if (x == width - 2)
		{
			test3.setPosition(x * 32.f, yPosition);
			test3.setTexture(TextureManager::GetTexture("test_3"));
		}
	}

	// For loop for digits
	for (unsigned int x = 0; x < widthPixels; x+= 21)
	{
		if (x == 0)
		{
			if (mineCounter < 0)
			{
				digit1.setPosition(x, yPosition);
				digit1.setTexture(TextureManager::GetTexture("digits", "-", 210, 0, 21, 32));
			}
		}
		else if (x == 21)
		{
			int hundreds;
			digit2.setPosition(x, yPosition);

			if (mineCounter < 0)
			{
				hundreds = (-1 * mineCounter) / 100;
			}
			else
				hundreds = mineCounter / 100;

			digit2.setTexture(TextureManager::GetTexture("digits", to_string(hundreds), hundreds * 21, 0, 21, 32));
		}
		else if (x == 42)
		{
			int tens;
			digit3.setPosition(x, yPosition);

			if (mineCounter < 0)
				tens = ((-1 * mineCounter) % 100) / 10;
			else
				tens = (mineCounter % 100) / 10;

			digit3.setTexture(TextureManager::GetTexture("digits", to_string(tens), tens * 21, 0, 21, 32));
		}
		else if (x == 63)
		{
			int ones;
			digit4.setPosition(x, yPosition);

			if (mineCounter < 0)
				ones = (-1 * mineCounter) % 10;
			else
				ones = mineCounter % 10;

			digit4.setTexture(TextureManager::GetTexture("digits", to_string(ones), ones * 21, 0, 21, 32));
		}
	}

	window.draw(smiley);
	window.draw(debug);
	window.draw(test1);
	window.draw(test2);
	window.draw(test3);

	window.draw(digit1);
	window.draw(digit2);
	window.draw(digit3);
	window.draw(digit4);
}

void Board::GameWon()
{
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			if (tiles[y][x]->Mine() && !tiles[y][x]->Flagged())
			{
				tiles[y][x]->ChangeFlagged();
				mineCounter--;
			}
		}
	}
}
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName);
	static void LoadTexture(string fileName, string append, int xPos, int yPos, int xSize, int ySize);

public:
	static sf::Texture& GetTexture(string textureName);
	static sf::Texture& GetTexture(string textureName, string append, int xPos, int yPos, int xSize, int ySize);
	static void Clear(); // Call once, at end of main()
};


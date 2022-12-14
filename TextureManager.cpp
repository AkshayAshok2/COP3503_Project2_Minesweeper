#include "TextureManager.h"
using namespace std;

// "Redeclare" var, so this file knows it exists and can use it
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	string path = "images/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

void TextureManager::LoadTexture(string fileName, string append, int xPos, int yPos, int xSize, int ySize)
{
	string path = "images/";
	path += fileName + ".png";

	textures[fileName + append].loadFromFile(path, sf::IntRect(xPos, yPos, xSize, ySize));
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	// If the texture doesn't exist...
	if (textures.find(textureName) == textures.end())
		LoadTexture(textureName);

	return textures[textureName];
}

sf::Texture& TextureManager::GetTexture(string textureName, string append, int xPos, int yPos, int xSize, int ySize)
{
	// If the texture doesn't exist...
	if (textures.find(textureName + append) == textures.end())
		LoadTexture(textureName, append, xPos, yPos, xSize, ySize);

	return textures[textureName + append];
}

void TextureManager::Clear()
{
	textures.clear(); // Get rid of stored objects
}


#pragma once

#include "Tile.h"
#include "Player.h"
#include "Enemy.h"

class TileMap
{
private:
	// To clear the contents of the vector
	void clear();

	// Texture of the tiles
	sf::Texture tile_Tex;
	// Grid size of each tile
	float gridSizeF;
	unsigned gridSizeU;
	// Max size of the total tiles
	sf::Vector2i maxSize;
	// Layers of the tiles to be placed on top of each other
	unsigned layers;
	int fromX, toX, fromY, toY, layer;

	std::vector< std::vector< std::vector< Tile* > > > tileMap;
	// File path of the texture file
	std::string texPath;

	sf::Clock movingClock;
	bool moveForward;

	sf::Clock breakingClock;
	bool breakTimerStarted;

	float keyTime;
	float keyTimeMax;

public:
	TileMap(float gridSize, unsigned width, unsigned height, std::string tex_path);
	~TileMap();

	// Map Editing
	void addToMap(const int x, const int y, const int z, sf::IntRect tex_change, bool collision, int type);
	void removeFromMap(const int x, const int y, const int z);
	void mapCulling(Entity* player, sf::Vector2i& viewPosGrid, const float& dt);
	void saveToFile(std::string path);
	void loadFromFile(std::string path);

	// Tiles Type Functions
	void movingTile(int x, int y, int z, const float& dt);
	void breakingTile(int x, int y, int z);

	const sf::Texture& getTileTex() const; 
	const bool getKeyTime();

	void updatePlatformCollision(Entity* player, int x, int y, int z);
	void updateKeyTime(const float& dt);

	void update(sf::Vector2f& mousePosView, Entity* player, sf::Vector2i& viewPosGrid, const float& dt);
	void render(sf::RenderTarget& target, sf::Vector2i& viewPosGrid);
};



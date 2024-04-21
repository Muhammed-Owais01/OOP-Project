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
	sf::Vector2u maxSize;
	// Layers of the tiles to be placed on top of each other
	unsigned layers;
	int fromX, toX, fromY, toY, layer;

	std::vector< std::vector< std::vector< Tile* > > > tileMap;
	// File path of the texture file
	std::string texPath;

	sf::Clock movingClock;
	bool moveForward;

	float keyTime;
	float keyTimeMax;

public:
	TileMap(float gridSize, unsigned width, unsigned height, std::string tex_path);
	~TileMap();

	void addToMap(const unsigned x, const unsigned y, const unsigned z, sf::IntRect tex_change, bool collision, int type);
	void removeFromMap(const unsigned x, const unsigned y, const unsigned z);
	void mapCulling(Entity* player, sf::Vector2i& viewPosGrid, const float& dt);
	void movingTile(int x, int y, int z, const float& dt);
	void saveToFile(std::string path);
	void loadFromFile(std::string path);

	const sf::Texture& getTileTex() const; 
	const bool getKeyTime();

	void updatePlatformCollision(Entity* player, int x, int y, int z);
	void updateKeyTime(const float& dt);

	void update(sf::Vector2f& mousePosView, Entity* player, sf::Vector2i& viewPosGrid, const float& dt);
	void render(sf::RenderTarget& target, sf::Vector2i& viewPosGrid);
};



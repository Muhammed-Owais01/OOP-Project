#pragma once

#include "Tile.h"

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
	std::vector< std::vector< std::vector< Tile* > > > tileMap;
	// File path of the texture file
	std::string texPath;

public:
	TileMap(float gridSize, unsigned width, unsigned height, std::string tex_path);
	~TileMap();

	void addToMap(const unsigned x, const unsigned y, const unsigned z, sf::IntRect tex_change);
	void removeFromMap(const unsigned x, const unsigned y, const unsigned z);
	void saveToFile(std::string path);
	void loadFromFile(std::string path);

	const sf::Texture& getTileTex() const;

	void update(sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};



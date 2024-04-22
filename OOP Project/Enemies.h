#pragma once

#include "Enemy.h"
class Enemies
{
private:
	std::string boar_tex_path;
	std::string bee_tex_path;
	std::string snail_tex_path;
	sf::Texture boar_tex;
	sf::Texture bee_tex;
	sf::Texture snail_tex;

	// Grid size of each enemy
	float gridSizeF;
	unsigned gridSizeU;
	// Max size of the total tiles
	sf::Vector2u maxSize;
	int layers;
	std::vector< std::vector< std::vector<Enemy*> > > enemyList;

	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;
public:
	Enemies(float gridSize, unsigned width, unsigned height);
	~Enemies();

	void addEnemy(const unsigned x, const unsigned y, const unsigned z, int type);
	void removeEnemy(const unsigned x, const unsigned y, const unsigned z);
	void saveToFile(std::string path);

	void update(sf::RenderWindow& window, sf::Vector2i& viewPosGrid);
	void render(sf::RenderTarget* target, sf::Vector2i& viewPosGrid);
};


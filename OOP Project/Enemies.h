#pragma once

#include "Enemy.h"
class Enemies
{
private:
	// To clear the contents of the vector
	void clear();
	float keyTime;
	float keyTimeMax;

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

	bool moveForward;
	sf::Clock movingClock;
public:
	Enemies(float gridSize, unsigned width, unsigned height);
	~Enemies();

	void addEnemy(const unsigned x, const unsigned y, const unsigned z, int type);
	void removeEnemy(const unsigned x, const unsigned y, const unsigned z);
	void EnemyCulling(Entity* player, sf::Vector2i& viewPosGrid, const float& dt);
	void saveToFile(std::string path);
	void loadFromFile(std::string path);
	void movingEnemy(int x, int y, int z, const float& dt);
	const bool getKeyTime();
	void updateKeyTime(const float& dt);

	void update(sf::RenderWindow& window, sf::Vector2i& viewPosGrid, const float dt, Entity* player);
	void render(sf::RenderTarget* target, sf::Vector2i& viewPosGrid);
};


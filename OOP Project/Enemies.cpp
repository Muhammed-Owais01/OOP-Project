#include "stdafx.h"
#include "Enemies.h"

Enemies::Enemies(float gridSize, unsigned width, unsigned height)
{
	this->gridSizeF = gridSize;
	this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
	this->maxSize.x = width;
	this->maxSize.y = height;
	this->layers = 1;
	this->layer = 0;
	this->fromX = 0;
	this->fromY = 0;
	this->toX = 0;
	this->toY = 0;
	this->boar_tex_path = "Textures/Enemy Textures/boar.png";
	this->bee_tex_path = "";
	this->snail_tex_path = "";

	this->enemyList.resize(this->maxSize.x, std::vector< std::vector <Enemy*> >());
	for (size_t x = 0; x < this->maxSize.x; x++)
	{
		for (size_t y = 0; y < this->maxSize.y; y++)
		{
			// Resizing at x to have a vector in y
			this->enemyList[x].resize(this->maxSize.y, std::vector<Enemy*>());
			for (size_t z = 0; z < this->layers; z++)
			{
				// Resizing to in x,y to have a vector in z
				// second parameter is nullptr, so all the locations are initialized to nullptr
				this->enemyList[x][y].resize(this->layers, nullptr);
			}
		}
	}
	if (!this->boar_tex.loadFromFile(this->boar_tex_path))
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD BOAR" << std::endl;
}

Enemies::~Enemies()
{

}

void Enemies::addEnemy(const unsigned x, const unsigned y, const unsigned z, int type)
{
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && this->maxSize.y))
	{
		if (type == ENEMY_TYPE::BOAR)
			this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, boar_tex);
	}
}

void Enemies::removeEnemy(const unsigned x, const unsigned y, const unsigned z)
{
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && this->maxSize.y))
	{
		delete this->enemyList[x][y][z];
		this->enemyList[x][y][z] = nullptr;
	}
}

void Enemies::saveToFile(std::string path)
{
	/*
	* Basic Data:
	*	- gridSize
	*	- maxSize x, y
	*	- layers
	*	- texture path boar
	*	- texture path bee
	*	- texture path snail
	*
	*  ALL:
	*	- x, y, z
	*	- type
	*/

	std::ofstream ofs(path);

	if (ofs.is_open())
	{
		ofs << this->gridSizeU << std::endl
			<< this->maxSize.x << " " << this->maxSize.y << std::endl
			<< this->layers << std::endl
			<< this->boar_tex_path << std::endl;


	}
}

void Enemies::update(sf::RenderWindow& window, sf::Vector2i& viewPosGrid)
{
	this->fromX = viewPosGrid.x - 17;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSize.x)
		this->fromX = maxSize.x;

	this->toX = viewPosGrid.x + 17;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSize.x)
		this->toX = maxSize.x;

	this->fromY = viewPosGrid.y - 30;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSize.y)
		this->fromY = maxSize.y;

	this->toY = viewPosGrid.y + 30;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSize.y)
		this->toY = maxSize.y;

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr)
				this->enemyList[x][y][this->layer]->update(window);
		}
	}
}

void Enemies::render(sf::RenderTarget* target, sf::Vector2i& viewPosGrid)
{
	// In essence these numbers load only the enemies that can be seen on the screen
	this->fromX = viewPosGrid.x - 17;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSize.x)
		this->fromX = maxSize.x;

	this->toX = viewPosGrid.x + 17;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSize.x)
		this->toX = maxSize.x;

	this->fromY = viewPosGrid.y - 30;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSize.y)
		this->fromY = maxSize.y;

	this->toY = viewPosGrid.y + 30;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSize.y)
		this->toY = maxSize.y;

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr)
				this->enemyList[x][y][this->layer]->render(target);
		}
	}
}

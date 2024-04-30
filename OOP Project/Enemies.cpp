#include "stdafx.h"
#include "Enemies.h"

void Enemies::clear()
{
	for (unsigned int x = 0; x < this->maxSize.x; x++)
	{
		for (unsigned int y = 0; y < this->maxSize.y; y++)
		{
			for (int z = 0; z < this->layers; z++)
			{
				// Delete references to it
				delete this->enemyList[x][y][z];
				this->enemyList[x][y][z] = nullptr;
			}
			// Clear the vector [x][y]
			this->enemyList[x][y].clear();
		}
		// Clear the vector [x]
		this->enemyList[x].clear();
	}
	// Clear the vector fully
	this->enemyList.clear();
}

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

	this->boar_tex_path = "Textures/Enemy_Textures/boar.png";
	this->bee_tex_path = "";
	this->snail_tex_path = "";

	// First intialize the whole vector
	this->enemyList.resize(this->maxSize.x, std::vector< std::vector <Enemy*> >());
	for (unsigned int x = 0; x < this->maxSize.x; x++)
	{
		for (unsigned int y = 0; y < this->maxSize.y; y++)
		{
			// Resizing at x to have a vector in y
			this->enemyList[x].resize(this->maxSize.y, std::vector<Enemy*>());
			for (int z = 0; z < this->layers; z++)
			{
				// Resizing to in x,y to have a vector in z
				// second parameter is nullptr, so all the locations are initialized to nullptr
				this->enemyList[x][y].resize(this->layers, nullptr);
			}
		}
	}
	// Load boar texture
	if (!this->boar_tex.loadFromFile(this->boar_tex_path))
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD BOAR" << std::endl;
}

Enemies::~Enemies()
{
	this->clear();
}

void Enemies::addEnemy(const unsigned x, const unsigned y, const unsigned z, int type)
{
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && this->maxSize.y))
	{
		// If enemy type is boar then give it the boar texture
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
			<< this->boar_tex_path << std::endl
			<< this->bee_tex_path << std::endl
			<< this->snail_tex_path << std::endl;

		for (size_t x = 0; x < this->maxSize.x; x++)
		{
			for (size_t y = 0; y < this->maxSize.y; y++)
			{
				for (int z = 0; z < this->layers; z++)
				{
					if (this->enemyList[x][y][z] != nullptr)
						ofs << x << " " << y << " " << z << " " << this->enemyList[x][y][z]->allToString() << " ";
				}
			}
		}
	}
	else
		std::cout << "ERROR::ENEMIES::COULD NOT SAVE TO FILE " << path << std::endl;
	ofs.close();
}

void Enemies::loadFromFile(std::string path)
{
	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		this->gridSizeU = 0;
		this->maxSize.x = 0;
		this->maxSize.y = 0;
		this->layers = 0;
		this->boar_tex_path = "";
		this->bee_tex_path = "";
		this->snail_tex_path = "";

		ifs >> this->gridSizeU >> this->maxSize.x >> this->maxSize.y >> this->layers >> this->boar_tex_path >> this->bee_tex_path >> this->snail_tex_path;

		this->gridSizeF = static_cast<float>(this->gridSizeU);

		this->clear();

		int x = 0;
		int y = 0;
		int z = 0;
		int type = 0;

		this->enemyList.resize(this->maxSize.x, std::vector< std::vector<Enemy*> >());
		for (unsigned int x = 0; x < this->maxSize.x; x++)
		{
			for (unsigned int y = 0; y < this->maxSize.y; y++)
			{
				this->enemyList[x].resize(this->maxSize.y, std::vector<Enemy*>());
				for (int z = 0; z < this->layers; z++)
				{
					// second parameter is nullptr, so all the locations are initialized to nullptr
					this->enemyList[x][y].resize(this->layers, nullptr);
				}
			}
		}
		if (!this->boar_tex.loadFromFile(this->boar_tex_path))
			std::cout << "ERROR::ENEMIES::COULD NOT LOAD BOAR" << std::endl;

		while (ifs >> x >> y >> z >> type)
		{
			this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, this->boar_tex);
		}
	}
	else
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD FROM FILE " << path << std::endl;
	ifs.close();
}

void Enemies::update(sf::RenderWindow& window, sf::Vector2i& viewPosGrid, const float& dt)
{
	// Update only the enemies within the view
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

	for (unsigned int x = fromX; x < toX; x++)
	{
		for (unsigned int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr)
				this->enemyList[x][y][this->layer]->update(window, dt);
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

	for (unsigned int x = fromX; x < toX; x++)
	{
		for (unsigned int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr)
				this->enemyList[x][y][this->layer]->render(target);
		}
	}
}

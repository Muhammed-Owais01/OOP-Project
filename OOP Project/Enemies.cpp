#include "stdafx.h"
#include "Enemies.h"

void Enemies::clear()
{
	for (size_t x = 0; x < this->maxSize.x; x++)
	{
		for (size_t y = 0; y < this->maxSize.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
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
	this->keyTime = 0;
	this->keyTimeMax = 100.f;
	this->moveForward = false;
	this->movingClock.restart();
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
	this->bee_tex_path = "Textures/Enemy_Textures/bee.png";
	this->snail_tex_path = "";

	// First intialize the whole vector
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
	// Load boar texture
	if (!this->boar_tex.loadFromFile(this->boar_tex_path))
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD BOAR" << std::endl;
	if (!this->bee_tex.loadFromFile(this->bee_tex_path))
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD BEE" << std::endl;
}

Enemies::~Enemies()
{
	this->clear();
}
void Enemies::EnemyCulling(Entity* player, sf::Vector2i& viewPosGrid, const float& dt)
{

	// Created a new one because I need the moving platforms to be updated even when player isnt near them
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

	this->fromY = viewPosGrid.y - 15;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSize.y)
		this->fromY = maxSize.y;

	this->toY = viewPosGrid.y + 15;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSize.y)
		this->toY = maxSize.y;

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr)
			{
					this->movingEnemy(x, y, this->layer, dt);
			}
		}
	}
}
void Enemies::addEnemy(const unsigned x, const unsigned y, const unsigned z, int type)
{
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && this->maxSize.y))
	{
		// If enemy type is boar then give it the boar texture
		if (type == ENEMY_TYPE::BOAR)
			this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, boar_tex);
		if (type == ENEMY_TYPE::BEE)
			this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, bee_tex);
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
				for (size_t z = 0; z < this->layers; z++)
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
		for (size_t x = 0; x < this->maxSize.x; x++)
		{
			for (size_t y = 0; y < this->maxSize.y; y++)
			{
				this->enemyList[x].resize(this->maxSize.y, std::vector<Enemy*>());
				for (size_t z = 0; z < this->layers; z++)
				{
					// second parameter is nullptr, so all the locations are initialized to nullptr
					this->enemyList[x][y].resize(this->layers, nullptr);
				}
			}
		}
		if (!this->boar_tex.loadFromFile(this->boar_tex_path))
			std::cout << "ERROR::ENEMIES::COULD NOT LOAD BOAR" << std::endl;
		if (!this->bee_tex.loadFromFile("Textures/Enemy_Textures/bee.png"))
			std::cout << "ERROR::ENEMIES::COULD NOT LOAD BEE WHEN LOADING FROM FILE" << std::endl;

		while (ifs >> x >> y >> z >> type)
		{
			if (type == ENEMY_TYPE::BOAR)
				this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, this->boar_tex);
			else if (type == ENEMY_TYPE::BEE)
				this->enemyList[x][y][z] = new Enemy(x, y, this->gridSizeF, type, this->bee_tex);
		}
	}
	else
		std::cout << "ERROR::ENEMIES::COULD NOT LOAD FROM FILE " << path << std::endl;
	ifs.close();
}
// Moves a tile if it is a tile of TILE_TYPE::MOVING
void Enemies::movingEnemy(int x, int y, int z, const float& dt)
{
	// Set to move forward or backwards as time goes
	if (this->movingClock.getElapsedTime().asSeconds() > 2.f)
	{
		this->movingClock.restart();
		if (this->moveForward)
			this->moveForward = false;
		else
			this->moveForward = true;
	}

	// Move the tile forward until a set amount of time and a set amount of space
	if (this->enemyList[x][y][z]->getPosition(1.f).x < this->enemyList[x][y][z]->getMaxPosRightX()
		&& this->movingClock.getElapsedTime().asSeconds() <= 2.f
		&& this->moveForward)
		this->enemyList[x][y][z]->moveEnemy(1.f);
	// Move the tile backwards until a set amount of time and a set amount of space
	else if (this->enemyList[x][y][z]->getPosition(1.f).x > this->enemyList[x][y][z]->getMaxPosLeftX()
		&& this->movingClock.getElapsedTime().asSeconds() <= 2.f
		&& !this->moveForward)
		this->enemyList[x][y][z]->moveEnemy(-1.f);
}
// Getter of keyTime
const bool Enemies::getKeyTime()
{
	// reset keyTime to 0 if it is greater than max
	if (this->keyTime > this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

// Update KeyTime Value
void Enemies::updateKeyTime(const float& dt)
{
	// add to keyTime if it is less than max
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
		//std::cout << this->keyTime << std::endl;
	}
}
void Enemies::update(sf::RenderWindow& window, sf::Vector2i& viewPosGrid, const float dt, Entity* player)
{
	if (player != nullptr)
	{
		this->updateKeyTime(dt);
		this->EnemyCulling(player, viewPosGrid, dt);
	}

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

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{
			if (this->enemyList[x][y][this->layer] != nullptr) {
				this->enemyList[x][y][this->layer]->playerAlive(*player);
				this->enemyList[x][y][this->layer]->update(window);
			}			
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

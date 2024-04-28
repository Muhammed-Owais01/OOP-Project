#include "stdafx.h"
#include "TileMap.h"

void TileMap::clear()
{
	for (size_t x = 0; x < this->maxSize.x; x++)
	{
		for (size_t y = 0; y < this->maxSize.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
			{
				// Delete references to it
				delete this->tileMap[x][y][z];
				this->tileMap[x][y][z] = nullptr;
			}
			// Clear the vector [x][y]
			this->tileMap[x][y].clear();
		}
		// Clear the vector [x]
		this->tileMap[x].clear();
	}
	// Clear the vector fully
	this->tileMap.clear();
}

TileMap::TileMap(float gridSize, unsigned width, unsigned height, std::string tex_path)
{
	this->gridSizeF = gridSize;
	this->gridSizeU = static_cast<float>(gridSizeF);
	this->maxSize.x = width;
	this->maxSize.y = height;
	this->layers = 1;
	this->texPath = tex_path;
	this->fromX = 0;
	this->fromY = 0;
	this->toX = 0;
	this->toY = 0;
	this->layer = 0;

	this->movingClock.restart();
	this->moveForward = true;
	this->breakTimerStarted = false;

	this->keyTime = 0;
	this->keyTimeMax = 100.f;

	// 2D or 3D vectors need to be resized, so i am resizing it first to have a 2d vector in it with maxSize.x
	this->tileMap.resize(this->maxSize.x, std::vector< std::vector<Tile*> >());
	for (size_t x = 0; x < this->maxSize.x; x++)
	{
		for (size_t y = 0; y < this->maxSize.y; y++)
		{
			// Resizing at x to have a vector in y
			this->tileMap[x].resize(this->maxSize.y, std::vector<Tile*>());
			for (size_t z = 0; z < this->layers; z++)
			{
				// Resizing to in x,y to have a vector in z
				// second parameter is nullptr, so all the locations are initialized to nullptr
				this->tileMap[x][y].resize(this->layers, nullptr);
			}
		}
	}
	if (!this->tile_Tex.loadFromFile(this->texPath))
		std::cout << "ERROR::TILEMAP::COULD NOT LOAD MAP" << std::endl;
}

TileMap::~TileMap()
{
	this->clear();
}

void TileMap::addToMap(const unsigned x, const unsigned y, const unsigned z, sf::IntRect tex_change, bool collision, int type)
{
	// If x,y is correct you can add in vector
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && y < this->maxSize.y))
	{
		// If the location is nullptr only then you can add
		if (this->tileMap[x][y][z] == nullptr)
		{
			// Add a tile at x,y,z
			this->tileMap[x][y][z] = new Tile(x, y, this->gridSizeF, this->tile_Tex, tex_change, collision, type);
		}
	}
}

void TileMap::removeFromMap(const unsigned x, const unsigned y, const unsigned z)
{
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && y < this->maxSize.y))
	{
		if (this->tileMap[x][y][z] != nullptr)
		{
			delete this->tileMap[x][y][z];
			this->tileMap[x][y][z] = nullptr;
		}
	}
}

void TileMap::mapCulling(Entity* player, sf::Vector2i& viewPosGrid, const float& dt)
{
	// Position of X and Y from which to update and Position of X and Y till which to update
	// In essence only tiles above 3, below 3, to the right of player 3 and to the left of player 3 will be updated
	this->fromX = viewPosGrid.x - 3;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSize.x)
		this->fromX = maxSize.x;

	this->toX = viewPosGrid.x + 3;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSize.x)
		this->toX = maxSize.x;

	this->fromY = viewPosGrid.y - 3;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSize.y)
		this->fromY = maxSize.y;

	this->toY = viewPosGrid.y + 3;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSize.y)
		this->toY = maxSize.y;

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{
			if (this->tileMap[x][y][this->layer] != nullptr)
			{
				if (this->tileMap[x][y][this->layer]->getCollision())
				{
					this->updatePlatformCollision(player, x, y, this->layer);
				}
			}
		}
	}

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
			if (this->tileMap[x][y][this->layer] != nullptr)
			{
				if (this->tileMap[x][y][this->layer]->getType() == TILE_TYPES::MOVING)
					this->movingTile(x, y, this->layer, dt);
			}


		}
	}
}

void TileMap::saveToFile(std::string path)
{
	/*
	* Basic Data:
	*	- gridSize
	*	- maxSize x, y
	*	- layers
	*	- texture path
	* 
	*  ALL:
	*	- x, y, z
	*	- texRect left, texRect top
	*	- collision
	*	- type
	*/

	// Open file for write at path
	std::ofstream ofs(path);

	// If file is open
	if (ofs.is_open())
	{
		// Output to the file the basic data
		ofs << this->gridSizeU << std::endl
			<< this->maxSize.x << " " << this->maxSize.y << std::endl
			<< this->layers << std::endl
			<< this->texPath << std::endl;

		for (size_t x = 0; x < this->maxSize.x; x++)
		{
			for (size_t y = 0; y < this->maxSize.y; y++)
			{
				for (size_t z = 0; z < this->layers; z++)
				{
					// If vector location isnt null then output to file x,y,z and the tile data
					if (this->tileMap[x][y][z] != nullptr)
						ofs << x << " " << y << " " << z << " " << this->tileMap[x][y][z]->allToString() << " ";
				}
			}
		}
	}
	else
		std::cout << "ERROR::TILEMAP::COULD NOT SAVE TO FILE " << path << std::endl;
	ofs.close();
}

void TileMap::loadFromFile(std::string path)
{
	// Open file for read at path
	std::ifstream ifs(path);

	// If file is open
	if (ifs.is_open())
	{
		this->gridSizeU = 0;
		this->maxSize.x = 0;
		this->maxSize.y = 0;
		this->layers = 0;
		this->texPath = "";

		// Extract from file and store the basic data in the appropriate variables
		ifs >> this->gridSizeU >> this->maxSize.x >> this->maxSize.y >> this->layers >> this->texPath;

		// Assign gridSizeF the value 
		this->gridSizeF = static_cast<float>(this->gridSizeU);

		// Clear the vectors first
		this->clear();

		// Variables to store data for x,y,z, texRect Left and texRect Top
		unsigned x = 0;
		unsigned y = 0;
		unsigned z = 0;
		int tRL = 0;
		int tRT = 0;
		bool collision;
		int type;

		// Initialize the vectors
		this->tileMap.resize(this->maxSize.x, std::vector< std::vector<Tile*> >());
		for (size_t x = 0; x < this->maxSize.x; x++)
		{
			for (size_t y = 0; y < this->maxSize.y; y++)
			{
				this->tileMap[x].resize(this->maxSize.y, std::vector<Tile*>());
				for (size_t z = 0; z < this->layers; z++)
				{
					// second parameter is nullptr, so all the locations are initialized to nullptr
					this->tileMap[x][y].resize(this->layers, nullptr);
				}
			}
		}
		if (!this->tile_Tex.loadFromFile(this->texPath))
			std::cout << "ERROR::TILEMAP::COULD NOT LOAD MAP" << std::endl;

		// Read from file the data until all data has been read
		while (ifs >> x >> y >> z >> tRL >> tRT >> collision >> type)
		{
			// At the correct location store the new tile
			this->tileMap[x][y][z] = new Tile(x, y, this->gridSizeF, this->tile_Tex, sf::IntRect(tRL, tRT, this->gridSizeU, this->gridSizeU), collision, type);
		}
	}
	else
		std::cout << "ERROR::TILEMAP::COULD NOT LOAD FROM FILE " << path << std::endl;
	ifs.close();
}

// Moves a tile if it is a tile of TILE_TYPE::MOVING
void TileMap::movingTile(int x, int y, int z, const float& dt)
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
	if (this->tileMap[x][y][z]->getPosition(1.f).x < this->tileMap[x][y][z]->getMaxPosRightX()
		&& this->movingClock.getElapsedTime().asSeconds() <= 2.f
		&& this->moveForward)
		this->tileMap[x][y][z]->moveTile(1.f);
	// Move the tile backwards until a set amount of time and a set amount of space
	else if (this->tileMap[x][y][z]->getPosition(1.f).x > this->tileMap[x][y][z]->getMaxPosLeftX()
		&& this->movingClock.getElapsedTime().asSeconds() <= 2.f
		&& !this->moveForward)
		this->tileMap[x][y][z]->moveTile(-1.f);
}

// Called in updatePlatformCollision as it can only work if a player is intersecting the tile
void TileMap::breakingTile(int x, int y, int z)
{
	// Start the timer for breaking the block
	if (!this->breakTimerStarted || this->breakingClock.getElapsedTime().asSeconds() > 3.1f)
	{
		this->breakingClock.restart();
		this->breakTimerStarted = true;
	}
	// Break the block by turning off its collision
	if (this->breakingClock.getElapsedTime().asSeconds() > 3.f)
	{
		this->tileMap[x][y][z]->setCollision(false);
		this->tileMap[x][y][z]->setHidden(true);
		this->breakTimerStarted = false;
	}
}


const sf::Texture& TileMap::getTileTex() const
{
	return this->tile_Tex;
}

// Getter of keyTime
const bool TileMap::getKeyTime()
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
void TileMap::updateKeyTime(const float& dt)
{
	// add to keyTime if it is less than max
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
		//std::cout << this->keyTime << std::endl;
	}
}

void TileMap::updatePlatformCollision(Entity* player, int x, int y, int z)
{
	sf::FloatRect playerBounds = player->getBounds();
	sf::FloatRect wallBounds = this->tileMap[x][y][z]->getGlobalBounds();
	// We predict the next player position using its velocity and then check if its colliding with the tile
	sf::FloatRect nextPositionBounds = sf::FloatRect(
		player->getPosition().x + player->getPlayerVelocity().x, player->getPosition().y + player->getPlayerVelocity().y, 40.f, 50.f);

	if (this->tileMap[x][y][z]->getGlobalBounds().intersects(nextPositionBounds))
	{
		//Bottom collision
		if (playerBounds.top < wallBounds.top
			&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
			&& playerBounds.left < wallBounds.left + wallBounds.width
			&& playerBounds.left + playerBounds.width > wallBounds.left
			)
		{
			player->setPlayerVelocityY(0);
			player->setPlayerPosition(playerBounds.left, wallBounds.top - playerBounds.height);
		}

		//Top collision
		else if (playerBounds.top > wallBounds.top
			&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
			&& playerBounds.left < wallBounds.left + wallBounds.width
			&& playerBounds.left + playerBounds.width > wallBounds.left
			)
		{
			player->setLanded(true);
			player->setPlayerVelocityY(0);
			player->setPlayerPosition(playerBounds.left, wallBounds.top + wallBounds.height);
		}

		//Right collision
		if (playerBounds.left < wallBounds.left
			&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
			&& playerBounds.top < wallBounds.top + wallBounds.height
			&& playerBounds.top + playerBounds.height > wallBounds.top
			)
		{
			player->setPlayerVelocityX(0);
			player->setPlayerPosition(wallBounds.left - playerBounds.width, playerBounds.top);
		}

		//Left collision
		else if (playerBounds.left > wallBounds.left
			&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
			&& playerBounds.top < wallBounds.top + wallBounds.height
			&& playerBounds.top + playerBounds.height > wallBounds.top
			)
		{
			player->setPlayerVelocityX(0);
			player->setPlayerPosition(wallBounds.left + wallBounds.width, playerBounds.top);
		}

		if (this->tileMap[x][y][z]->getType() == TILE_TYPES::BREAKING)
			this->breakingTile(x, y, z);
	}
	else {
		player->setLanded(false);
	}
}

void TileMap::update(sf::Vector2f& mousePosView, Entity* player, sf::Vector2i& viewPosGrid, const float& dt)
{
	if (player != nullptr)
	{
		this->updateKeyTime(dt);
		this->mapCulling(player, viewPosGrid, dt);
	}

}

// Render all the tiles
void TileMap::render(sf::RenderTarget& target, sf::Vector2i& viewPosGrid)
{
	// In essence these numbers loads only the tiles that can be seen on the screen
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
			if (this->tileMap[x][y][this->layer] != nullptr && !this->tileMap[x][y][this->layer]->getHidden())
				this->tileMap[x][y][this->layer]->render(target);
		}
	}

}

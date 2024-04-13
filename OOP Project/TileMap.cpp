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

void TileMap::addToMap(const unsigned x, const unsigned y, const unsigned z, sf::IntRect tex_change)
{
	// If x,y is correct you can add in vector
	if ((x >= 0 && x < this->maxSize.x) &&
		(y >= 0 && y < this->maxSize.y))
	{
		// If the location is nullptr only then you can add
		if (this->tileMap[x][y][z] == nullptr)
			// Add a tile at x,y,z
			this->tileMap[x][y][z] = new Tile(x, y, this->gridSizeF, this->tile_Tex, tex_change);
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
		while (ifs >> x >> y >> z >> tRL >> tRT)
		{
			// At the correct location store the new tile
			this->tileMap[x][y][z] = new Tile(x, y, this->gridSizeF, this->tile_Tex, sf::IntRect(tRL, tRT, this->gridSizeU, this->gridSizeU));
		}
	}
	else
		std::cout << "ERROR::TILEMAP::COULD NOT LOAD FROM FILE " << path << std::endl;
	ifs.close();
}

const sf::Texture& TileMap::getTileTex() const
{
	return this->tile_Tex;
}

void TileMap::update(sf::Vector2f& mousePosView)
{
	// Iterate through the whole vector and update the tiles
	for (auto& x : this->tileMap)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				if (z != nullptr)
					z->update(mousePosView);
			}
		}
	}
}

// Render all the tiles
void TileMap::render(sf::RenderTarget& target)
{
	for (auto& x : this->tileMap)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				if (z != nullptr)
					z->render(target);
			}
		}
	}
}

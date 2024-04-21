#include "stdafx.h"
#include "Tile.h"

Tile::Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, sf::IntRect tex_change, bool collision, int type)
	: collision(collision), type(type)
{
	// Setting tile properties
	this->tile.setFillColor(sf::Color::White);
	this->tile.setSize(sf::Vector2f(gridSize, gridSize));
	this->tile.setPosition(sf::Vector2f(x * gridSize, y * gridSize));
	// Giving the tile a texture
	this->tile.setTexture(&texture);
	/* Giving it a tex rect, basically think of it like a limiter on the texture,
	that it can load certain parts of the texture with different top left values */
	this->tile.setTextureRect(tex_change);

	this->initialPosX = this->tile.getPosition().x;
	this->initialPosY = this->tile.getPosition().y;

	this->maxPosRightX = this->initialPosX + gridSize * 2;
	this->maxPosLeftX = this->initialPosX - gridSize * 2;
}

Tile::~Tile()
{
}

const bool& Tile::getCollision() const
{
	return this->collision;
}

const int& Tile::getType() const
{
	return this->type;
}

const sf::Vector2f Tile::getPosition(const float gridSize)
{
	return this->tile.getPosition() / gridSize;
}

const float Tile::getMaxPosRightX() const
{
	return this->maxPosRightX;
}

const float Tile::getMaxPosLeftX() const
{
	return this->maxPosLeftX;
}

const sf::FloatRect& Tile::getGlobalBounds() const
{
	return this->tile.getGlobalBounds();
}

// Converting the different components of the tile into string for saving to file
const std::string Tile::allToString() const
{
	std::stringstream ss;
	ss << this->tile.getTextureRect().left << " " << this->tile.getTextureRect().top << " " << this->collision << " " << this->type;

	return ss.str();
}

void Tile::moveTile(float dir_x)
{
	this->tile.move(dir_x * 1.f, 0.f);
	//std::cout << "X: " << this->tile.getPosition().x << std::endl;
}

void Tile::update(sf::Vector2f& mousePosView)
{

}

void Tile::render(sf::RenderTarget& target)
{
	target.draw(this->tile);
}
// temp

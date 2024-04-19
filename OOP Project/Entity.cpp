#include "stdafx.h"
#include "Entity.h"

Entity::Entity(std::string path, std::string className)
{
	if (!this->texture.loadFromFile(path))
		std::cout << "ERROR::" << className <<  "::Could not load texture sheet" << std::endl;
	this->entity.setTexture(texture);
}

Entity::~Entity()
{
}

const sf::FloatRect& Entity::getBounds() const
{
	return this->entity.getGlobalBounds();
}

const sf::Vector2f& Entity::getPosition() const
{
	return this->entity.getPosition();
}
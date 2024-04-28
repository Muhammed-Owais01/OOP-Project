#include "stdafx.h"
#include "Entity.h"

Entity::Entity(sf::Texture& entity_tex, std::string className)
{
	this->entity.setTexture(entity_tex);
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

void Entity::setLanded(bool land)
{
}




#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(float x, float y, float gridSize, int type, sf::Texture& enemy_texture)
	: Entity(enemy_texture, "ENEMY"), type(type)
{
	this->entity.setPosition(sf::Vector2f(x * gridSize, y * gridSize));
	this->enemyAnimation = new Animation(&this->entity, 45, 27);
}

Enemy::~Enemy()
{
}

void Enemy::setPlayerPosition(float x, float y)
{
}

void Enemy::setPlayerVelocityY(float y)
{
}

void Enemy::setPlayerVelocityX(float x)
{
}

const sf::Vector2i& Enemy::getPlayerVelocity()
{
	return sf::Vector2i();
}

const std::string Enemy::allToString() const
{
	std::stringstream ss;
	ss << type;
	return ss.str();
}

void Enemy::updateMovement(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
	{
		this->entity.move(-1.f, 0.f);
		this->enemyAnimation->updateAnimations(ENTITY_ANIMATION_STATE::MOVING_LEFT, 27.f, 45.f, 270.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
	{
		this->entity.move(1.f, 0.f);
		this->enemyAnimation->updateAnimations(ENTITY_ANIMATION_STATE::MOVING_RIGHT, 27.f, 45.f, 270.f);
	}
	else
		this->enemyAnimation->updateAnimations(ENTITY_ANIMATION_STATE::IDLE, 0.f, 45.f, 140.f);
}

void Enemy::updateWindowCollision(sf::RenderWindow& window)
{

}

void Enemy::update(sf::RenderWindow& window, const float& dt)
{
	this->updateMovement(dt);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->entity);
}

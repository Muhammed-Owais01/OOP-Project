#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(float x, float y, float gridSize, int type, sf::Texture& enemy_texture)
	: Entity(enemy_texture, "ENEMY"), type(type)
{
	this->entity.setPosition(sf::Vector2f(x * gridSize, y * gridSize));
	this->enemyAnimation = new Animation(&this->entity, 45, 27);

	this->initialPosX = this->entity.getPosition().x;
	this->initialPosY = this->entity.getPosition().y;

	this->maxPosRightX = this->initialPosX + gridSize * 2;
	this->maxPosLeftX = this->initialPosX - gridSize * 2;
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

const sf::Vector2f Enemy::getPosition(const float gridSize)
{
	return this->entity.getPosition() / gridSize;
}

const float Enemy::getMaxPosRightX() const
{
	return maxPosRightX;
}

const float Enemy::getMaxPosLeftX() const
{
	return maxPosLeftX;
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

void Enemy::moveEnemy(float dir_x)
{
	this->entity.move(dir_x * 1.f, 0.f);
}

void Enemy::updateMovement()
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

void Enemy::update(sf::RenderWindow& window)
{
	this->updateMovement();
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->entity);
}

#include "stdafx.h"
#include "Enemy.h"

void Enemy::initVariables(sf::RenderWindow& window)
{
	this->platform = Platform(window);
	/*this->enemy.setFillColor(sf::Color::Yellow);
	this->enemy.setSize(sf::Vector2f(10.f, 10.f));*/
	if (!this->texSheet.loadFromFile("Textures/Player Textures/Anime Warrior/Anime Warrior/Warrior/Idle.png"))
		std::cout << "ERROR::enemy::Could not load player_sheet.png" << std::endl;

	this->enemy.setTexture(this->texSheet);
	this->enemy.setScale(1.1f, 1.1f);
}

Enemy::Enemy(sf::RenderWindow& window)
{
	this->initVariables(window);
}

Enemy::~Enemy()
{

}

const sf::FloatRect& Enemy::getBounds() const
{
	return this->enemy.getGlobalBounds();
}

const sf::Vector2f& Enemy::getPosition() const
{
	return this->enemy.getPosition();
}

const sf::Vector2i& Enemy::getPlayerVelocity()
{
	return static_cast<sf::Vector2i>(this->getVelocity());
}

void Enemy::setPlayerPosition(float x, float y)
{
	this->enemy.setPosition(x, y);
}

void Enemy::updateMovement()
{
	// JUMP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		this->move(0.f, -1.f);
	}
	// Move Backwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(-1.f, 0.f);
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->player.move(0.f, 1.f);
	}*/
	// Move Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->move(1.f, 0.f);
	}
	// Jump Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(1.f, -1.f);
	}
	else
		//this->playerAnimation->updateAnimations(0);
		this->enemy.move(this->getVelocity());

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	//{
	//	this->player.move(0.f, -10.f);
	//}
	//// Move Backwards
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	//{
	//	this->player.move(-10.f, 0.f);
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	//{
	//	this->player.move(0.f, 10.f);
	//}
	//// Move Forward
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	//{
	//	this->player.move(10.f, 0.f);
	//}
}

void Enemy::updateWindowCollision(sf::RenderWindow& window)
{
	sf::FloatRect bounds = this->enemy.getGlobalBounds();
	if (bounds.top < 0.f)
		this->enemy.setPosition(this->enemy.getPosition().x, 0.f);
	else if (bounds.top + bounds.height > window.getSize().y)
		this->enemy.setPosition(this->enemy.getPosition().x, window.getSize().y - bounds.height);

	if (bounds.left < 0.f)
		this->enemy.setPosition(0.f, this->enemy.getPosition().y);
	else if (bounds.left + bounds.width > window.getSize().x)
		this->enemy.setPosition(window.getSize().x - bounds.width, this->enemy.getPosition().y);
}

void Enemy::updatePlatformCollision()
{
	sf::Vector2f prevPlayerPos = this->enemy.getPosition();
	sf::FloatRect newPlayerPos = this->enemy.getGlobalBounds();

	float playerLeft = newPlayerPos.left;
	float playerRight = newPlayerPos.left + newPlayerPos.width;
	float playerTop = newPlayerPos.top;
	float playerBottom = newPlayerPos.top + newPlayerPos.height;

	float platformLeft = this->platform.getBounds().left;
	float platformRight = this->platform.getBounds().left + this->platform.getBounds().width;
	float platformTop = this->platform.getBounds().top;
	float platformBottom = this->platform.getBounds().top + this->platform.getBounds().height;


	if (newPlayerPos.intersects(this->platform.getBounds()))
	{
		// Box Top
		if ((playerBottom >= platformTop && playerBottom < platformBottom)
			&& (playerRight >= platformLeft + std::abs(this->getVelocity().x * 2) && playerLeft < platformRight - std::abs(this->getVelocity().x * 2)))
			this->enemy.setPosition(this->enemy.getPosition().x, this->platform.getBounds().top - newPlayerPos.height);

		// Box Left
		else if ((playerRight > platformLeft && playerRight < platformRight)
			&& (playerBottom >= platformTop + std::abs(this->getVelocity().y * 2) && playerTop <= platformBottom - std::abs(this->getVelocity().y * 2)))
			this->enemy.setPosition(this->platform.getBounds().left - newPlayerPos.width, this->enemy.getPosition().y);

		// Box Bottom
		else if ((playerTop < platformBottom && playerBottom > platformTop)
			&& (playerRight >= platformLeft + std::abs(this->getVelocity().x/* * 2*/) && playerLeft <= platformRight - std::abs(this->getVelocity().x/* * 2*/)))
			this->enemy.setPosition(this->enemy.getPosition().x, this->platform.getBounds().top + this->platform.getBounds().height);

		// Box Right
		else if ((playerLeft < platformRight && playerRight > platformLeft + std::abs(this->getVelocity().x * 2))
			&& (playerBottom >= platformTop && playerTop <= platformBottom))
			this->enemy.setPosition(this->platform.getBounds().left + this->platform.getBounds().width, this->enemy.getPosition().y);
	}
}

void Enemy::move(const float dir_x, const float dir_y)
{
	this->changeVelocity(dir_x * this->getAcceleration(), 0.f);

	if (std::abs(this->getVelocity().x) > this->getVelocityMax())
	{
		this->getVelocity().x = this->getVelocityMax() * ((this->getVelocity().x < 0) ? -1.f : 1.f);
	}

	this->getVelocity().y = dir_y * this->getJump();
}

void Enemy::update(sf::RenderWindow& window)
{
	this->updateMovement();
	this->updatePhysics();
	this->updateWindowCollision(window);
	this->updatePlatformCollision();
}

void Enemy::render(sf::RenderTarget* target)
{
	this->platform.render(target);
	target->draw(this->enemy);
}

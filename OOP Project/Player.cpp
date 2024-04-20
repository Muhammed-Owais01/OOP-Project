#include "stdafx.h"
#include "Player.h"

void Player::initVariables(sf::RenderWindow& window)
{
	// Platform is used for testing of collision
	//this->platform = Platform(window);

	this->entity.setScale(1.1f, 1.1f);
	this->playerAnimation = new Animation(&this->entity, 40, 50);


	this->hitBox.setSize(sf::Vector2f(this->entity.getGlobalBounds().getSize().x, this->entity.getGlobalBounds().getSize().y));
	this->hitBox.setPosition(this->entity.getPosition());
	this->hitBox.setFillColor(sf::Color(255, 0, 0, 50));
}

Player::Player(sf::RenderWindow& window)
	: Entity("Textures/Player Textures/Anime Warrior/Anime Warrior/Warrior/player_state.png", "Player")
{
	this->initVariables(window);
}

Player::~Player()
{

}

const sf::FloatRect& Player::getBounds() const
{
	return this->entity.getGlobalBounds();
}

const sf::Vector2f& Player::getPosition() const
{
	return this->entity.getPosition();
}

const sf::Vector2i& Player::getPlayerVelocity()
{
	return static_cast<sf::Vector2i>(this->getVelocity());
}

void Player::setPlayerPosition(float x, float y)
{
	this->entity.setPosition(x, y);
}

void Player::setPlayerVelocityY(float y)
{
	this->setVelocityY(y);
}

void Player::setPlayerVelocityX(float x)
{
	this->setVelocityX(x);
}

void Player::updateMovement()
{
	//// JUMP
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	//{
	//	this->move(0.f, -1.f);
	//}
	// Move Backwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(-1.f, 0.f);
		// Dont change this at all
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_LEFT, 50.f, 40.f, 316.f);
	}
	///*if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	//{
	//	this->entity.move(0.f, 1.f);
	//}*/
	// Move Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->move(1.f, 0.f);
		// Dont change this at all
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_RIGHT, 50.f, 40.f, 316.f);
	}
	// Jump Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(1.f, -1.f);
	}
	else
		// Dont change this at all
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::IDLE, 0.f, 40.f, 155.f);
	this->entity.move(this->getVelocity());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->entity.move(0.f, -10.f);
	}
	//// Move Backwards
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	//{
	//	this->entity.move(-10.f, 0.f);
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_LEFT, 50.f, 40.f, 316.f);
	//}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->entity.move(0.f, 10.f);
	}
	//// Move Forward
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	//{
	//	this->entity.move(10.f, 0.f);
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_RIGHT, 50.f, 40.f, 316.f);
	//}
	//else
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::IDLE, 0.f, 40.f, 155.f);
}

void Player::updateWindowCollision(sf::RenderWindow& window)
{
	sf::FloatRect bounds = this->entity.getGlobalBounds();
	if (bounds.top < 0.f)
		this->entity.setPosition(this->entity.getPosition().x, 0.f);
	else if (bounds.top + bounds.height > window.getSize().y)
		this->entity.setPosition(this->entity.getPosition().x, window.getSize().y - bounds.height);

	if (bounds.left < 0.f)
		this->entity.setPosition(0.f, this->entity.getPosition().y);
	else if (bounds.left + bounds.width > window.getSize().x)
		this->entity.setPosition(window.getSize().x - bounds.width, this->entity.getPosition().y);
}

void Player::updatePlatformCollision()
{
	sf::FloatRect playerBounds = this->entity.getGlobalBounds();
	sf::FloatRect wallBounds = this->platform.getBounds();
	sf::FloatRect nextPositionBounds = sf::FloatRect(
		this->entity.getPosition().x + this->getVelocity().x, this->entity.getPosition().y + this->getVelocity().y, 40.f, 50.f);

	this->hitBox.setPosition(sf::Vector2f(nextPositionBounds.left, nextPositionBounds.top));

	if (this->platform.getBounds().intersects(nextPositionBounds))
	{
		//Bottom collision
		if (playerBounds.top < wallBounds.top
			&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
			&& playerBounds.left < wallBounds.left + wallBounds.width
			&& playerBounds.left + playerBounds.width > wallBounds.left
			)
		{
			this->setVelocityY(0);
			this->entity.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
		}

		//Top collision
		else if (playerBounds.top > wallBounds.top
			&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
			&& playerBounds.left < wallBounds.left + wallBounds.width
			&& playerBounds.left + playerBounds.width > wallBounds.left
			)
		{
			this->setVelocityY(0);
			this->entity.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
		}

		//Right collision
		if (playerBounds.left < wallBounds.left
			&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
			&& playerBounds.top < wallBounds.top + wallBounds.height
			&& playerBounds.top + playerBounds.height > wallBounds.top
			)
		{
			this->setVelocityX(0);
			this->entity.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
		}

		//Left collision
		else if (playerBounds.left > wallBounds.left
			&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
			&& playerBounds.top < wallBounds.top + wallBounds.height
			&& playerBounds.top + playerBounds.height > wallBounds.top
			)
		{
			this->setVelocityX(0);
			this->entity.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
		}
	}
}

void Player::move(const float dir_x, const float dir_y)
{
	this->changeVelocity(dir_x * this->getAcceleration(), 0.f);

	if (std::abs(this->getVelocity().x) > this->getVelocityMax())
	{
		this->getVelocity().x = this->getVelocityMax() * ((this->getVelocity().x < 0) ? -1.f : 1.f);
	}

	this->getVelocity().y = dir_y * this->getJump();
}

void Player::update(sf::RenderWindow& window)
{
	this->updateMovement();
	this->updatePhysics();
	this->updateWindowCollision(window);
	this->updatePlatformCollision();

}

void Player::render(sf::RenderTarget* target)
{
	this->platform.render(target);
	// Dont change this at all
	this->playerAnimation->render(*target);
}

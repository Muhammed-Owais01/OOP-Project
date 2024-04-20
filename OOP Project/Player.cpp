#include "stdafx.h"
#include "Player.h"

void Player::initVariables(sf::RenderWindow& window)
{
	this->platform = Platform(window);
	/*this->player.setFillColor(sf::Color::Yellow);
	this->player.setSize(sf::Vector2f(10.f, 10.f));*/
	/*if (!this->texSheet.loadFromFile("Textures/Player Textures/Anime Warrior/Anime Warrior/Warrior/Idle.png"))
		std::cout << "ERROR::player::Could not load player_sheet.png" << std::endl;

	this->player.setTexture(this->texSheet);*/
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

void Player::updateMovement()
{
	//// JUMP
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	//{
	//	this->move(0.f, -1.f);
	//}
	//// Move Backwards
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	//{
	//	this->move(-1.f, 0.f);
	//	// Dont change this at all
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_LEFT, 50.f, 40.f, 316.f);
	//}
	///*if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	//{
	//	this->entity.move(0.f, 1.f);
	//}*/
	//// Move Forward
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	//{
	//	this->move(1.f, 0.f);
	//	// Dont change this at all
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_RIGHT, 50.f, 40.f, 316.f);
	//}
	//// Jump Forward
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	//{
	//	this->move(1.f, -1.f);
	//}
	//else
	//	// Dont change this at all
	//	this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::IDLE, 0.f, 40.f, 155.f);
	//this->entity.move(this->getVelocity());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->entity.move(0.f, -10.f);
	}
	// Move Backwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->entity.move(-10.f, 0.f);
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_LEFT, 50.f, 40.f, 316.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->entity.move(0.f, 10.f);
	}
	// Move Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->entity.move(10.f, 0.f);
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::MOVING_RIGHT, 50.f, 40.f, 316.f);
	}
	else
		this->playerAnimation->updateAnimations(PLAYER_ANIMATION_STATE::IDLE, 0.f, 40.f, 155.f);
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
	sf::Vector2i nextPos;
	sf::Vector2f prevPlayerPos = this->entity.getPosition();
	sf::FloatRect newPlayerPos = this->entity.getGlobalBounds();

	float playerLeft = newPlayerPos.left;
	float playerRight = newPlayerPos.left + newPlayerPos.width;
	float playerTop = newPlayerPos.top;
	float playerBottom = newPlayerPos.top + newPlayerPos.height;

	float platformLeft = this->platform.getBounds().left;
	float platformRight = this->platform.getBounds().left + this->platform.getBounds().width;
	float platformTop = this->platform.getBounds().top;
	float platformBottom = this->platform.getBounds().top + this->platform.getBounds().height;

	nextPos.x = std::abs(this->getVelocity().x * 2);
	nextPos.y = std::abs(this->getVelocity().y * 2);
	//this->hitBox.setPosition(sf::Vector2f((nextPos.x ));

	if (newPlayerPos.intersects(this->platform.getBounds()))
	{
		// Box Top
		if ((playerBottom >= platformTop && playerBottom < platformBottom)
			&& (playerRight >= platformLeft + nextPos.x && playerLeft < platformRight - nextPos.x))
			this->entity.setPosition(this->entity.getPosition().x, this->platform.getBounds().top - newPlayerPos.height);

		// Box Left
		else if ((playerRight > platformLeft && playerRight < platformRight)
			&& (playerBottom >= platformTop + nextPos.y && playerTop <= platformBottom - nextPos.y))
			this->entity.setPosition(this->platform.getBounds().left - newPlayerPos.width, this->entity.getPosition().y);

		// Box Bottom
		else if ((playerTop < platformBottom && playerBottom > platformTop)
			&& (playerRight >= platformLeft + nextPos.x && playerLeft <= platformRight - nextPos.x))
			this->entity.setPosition(this->entity.getPosition().x, this->platform.getBounds().top + this->platform.getBounds().height);

		// Box Right
		else if ((playerLeft < platformRight && playerRight > platformLeft + nextPos.y)
			&& (playerBottom >= platformTop && playerTop <= platformBottom))
			this->entity.setPosition(this->platform.getBounds().left + this->platform.getBounds().width, this->entity.getPosition().y);
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
	//this->updatePhysics();
	//this->updateWindowCollision(window);
	this->updatePlatformCollision();

}

void Player::render(sf::RenderTarget* target)
{
	this->platform.render(target);
	// Dont change this at all
	this->playerAnimation->render(*target);
	target->draw(this->hitBox);
}

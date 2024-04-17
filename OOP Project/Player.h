#pragma once

#include "Platform.h"
#include "Animation.h"

class Player
	: public GameEngine
{
private:
	sf::Sprite circle;

	sf::Texture texture;

	Animation* playerAnimation;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
	void initAnimation();
public:
	Player();
	Player(sf::RenderWindow& window);
	~Player();

	const sf::FloatRect& getBounds() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2i& getPlayerVelocity();

	void setPlayerPosition(float x, float y);
	void updateWindowCollision(sf::RenderWindow& window);
	void updatePlatformCollision();
	void move(const float dir_x, const float dir_y);
	void updateMovement();

	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget *target);
};


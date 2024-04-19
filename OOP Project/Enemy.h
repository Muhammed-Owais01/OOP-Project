#pragma once

#include "Platform.h"

class Enemy
	: public GameEngine
{
private:
	sf::Sprite enemy;
	sf::Texture texSheet;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
public:
	Enemy(sf::RenderWindow& window);
	~Enemy();

	const sf::FloatRect& getBounds() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2i& getPlayerVelocity();

	void setPlayerPosition(float x, float y);
	void updateMovement();
	void updateWindowCollision(sf::RenderWindow& window);
	void updatePlatformCollision();
	void move(const float dir_x, const float dir_y);


	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget* target);
};


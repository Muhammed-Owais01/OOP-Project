#pragma once

#include "Entity.h"
#include "Platform.h"
#include "Animation.h"

class Player
	: public GameEngine, public Entity
{
private:
	// Variables for projectile motion
	float gravity; // Gravity constant
	float velocityX;  // Initial velocity in x-direction
	float velocityY; // Initial velocity in y-direction
	float initialJumpVelocity; // Initial jump velocity
	float jumpLength; // Adjust this value to increase jump length

	bool isJumping,landed; // Flag to track if a jump is in progress

	// Time delay before jump
	sf::Clock jumpDelayClock;
	sf::Time jumpDelayTime;
	float originalY;

	sf::Texture player_texture;
	Animation* playerAnimation;

	sf::RectangleShape hitBox;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
public:
	Player(sf::RenderWindow& window);
	~Player();

	const sf::FloatRect& getBounds() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2i& getPlayerVelocity();

	void setPlayerPosition(float x, float y);
	void setPlayerVelocityY(float y);
	void setPlayerVelocityX(float x);
	void setLanded(bool land);
	void setOriginalY(float originalY);
	void updateMovement();
	void updateWindowCollision(sf::RenderWindow& window);
	
	//void updatePlatformCollision();
	//void move(const float dir_x, const float dir_y);


	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget* target);
};


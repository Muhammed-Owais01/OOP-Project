#pragma once

#include "State.h"

class GameEngine
{
protected:
	//// Variables for projectile motion
	//float gravity ; // Gravity constant
	//float velocityX ;  // Initial velocity in x-direction
	//float velocityY  ; // Initial velocity in y-direction
	//float initialJumpVelocity ; // Initial jump velocity
	//float jumpLength ; // Adjust this value to increase jump length

	//bool isJumping ; // Flag to track if a jump is in progress

	//// Time delay before jump
	//sf::Clock jumpDelayClock;
	//sf::Time jumpDelayTime;
	//float originalY;


	sf::Vector2f velocity;
	float velocityMin, velocityMax, acceleration,gravity, drag, velocityMaxY, jump;
public:
	GameEngine();
	~GameEngine();

	// Getters
	sf::Vector2f& getVelocity();
	const float& getVelocityMax() const;
	const float& getAcceleration() const;
	const float& getJump() const;

	// Setters
	void changeVelocity(float x, float y);
	void setVelocityX(float x);
	void setVelocityY(float y);

	virtual void initPhysics();
	void updatePhysics();

};


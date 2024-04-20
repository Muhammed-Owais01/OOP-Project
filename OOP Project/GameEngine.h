#pragma once

#include "State.h"

class GameEngine
{
private:
	sf::Vector2f velocity;
	float velocityMin, velocityMax, acceleration, gravity, drag, velocityMaxY, jump;
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


#pragma once

#include "Platform.h"
#include "Entity.h"
#include "Animation.h"

enum ENEMY_TYPE { BOAR = 0, BEE, SNAIL };
class Enemy
	: public GameEngine, public Entity
{
private:
	Animation* enemyAnimation;

	int type;
public:
	Enemy(float x, float y, float gridSize, int type, sf::Texture& enemy_texture);
	~Enemy();

	void setPlayerPosition(float x, float y);
	void setPlayerVelocityY(float y);
	void setPlayerVelocityX(float x);
	const sf::Vector2i& getPlayerVelocity();
	const std::string allToString() const;

	void updateMovement(const float& dt);
	void updateWindowCollision(sf::RenderWindow& window);
	//virtual void updatePlatformCollision() = 0;
	void update(sf::RenderWindow& window, const float& dt);
	void render(sf::RenderTarget* target);
};


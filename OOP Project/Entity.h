#pragma once
class Entity
{
protected:
	sf::Sprite entity;

public:
	Entity(sf::Texture& entity_tex, std::string className);
	~Entity();

	const sf::FloatRect& getBounds() const;
	const sf::Vector2f& getPosition() const; 
	virtual void setPlayerPosition(float x, float y) = 0;
	virtual void setPlayerVelocityY(float y) = 0;
	virtual void setPlayerVelocityX(float x) = 0;
	virtual void setLanded(bool land);
	virtual void setHp();
	virtual void setOriginalY(float originalY);
	virtual const sf::Vector2i& getPlayerVelocity() = 0;

	virtual void updateMovement() = 0;
	virtual void updateWindowCollision(sf::RenderWindow& window) = 0;
	//virtual void updatePlatformCollision() = 0;
	virtual void update(sf::RenderWindow& window) = 0;
	virtual void render(sf::RenderTarget* target) = 0;
};


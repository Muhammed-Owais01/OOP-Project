#pragma once
class Entity
{
protected:
	sf::Sprite entity;
	sf::Texture texture;

public:
	Entity(std::string path, std::string className);
	~Entity();

	const sf::FloatRect& getBounds() const;
	const sf::Vector2f& getPosition() const; 
	virtual void setPlayerPosition(float x, float y) = 0;
	virtual void setPlayerVelocityY(float y) = 0;
	virtual void setPlayerVelocityX(float x) = 0;
	virtual const sf::Vector2i& getPlayerVelocity() = 0;

	virtual void updateMovement() = 0;
	virtual void updateWindowCollision(sf::RenderWindow& window) = 0;
	//virtual void updatePlatformCollision() = 0;
	virtual void update(sf::RenderWindow& window) = 0;
	virtual void render(sf::RenderTarget* target) = 0;
};


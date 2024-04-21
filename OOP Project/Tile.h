#pragma once

enum TILE_TYPES {DEFAULT = 0, MOVING, BREAKING, SPIKES};
class Tile
{
private:
	// Basic tile on which texture is loaded
	sf::RectangleShape tile;

	float initialPosX;
	float maxPosRightX;
	float maxPosLeftX;
	float initialPosY;

	bool collision;
	int type;
public:
	Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, sf::IntRect tex_change, bool collision, int type);
	~Tile();

	const bool& getCollision() const;
	const int& getType() const;
	const sf::Vector2f getPosition(const float gridSize);
	const float getMaxPosRightX() const;
	const float getMaxPosLeftX() const;
	const sf::FloatRect& getGlobalBounds() const;
	const std::string allToString() const;

	void setCollision(bool collision);

	void moveTile(float dir_x);

	void update(sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};


#pragma once

enum TILE_TYPES {DEFAULT = 0, MOVING, BREAKING, SPIKES};
class Tile
{
private:
	// Basic tile on which texture is loaded
	sf::RectangleShape tile;

	bool collision;
	int type;
public:
	Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, sf::IntRect tex_change, bool collision, int type);
	~Tile();

	const bool& getCollision() const;
	const sf::FloatRect& getGlobalBounds() const;
	const std::string allToString() const;

	void update(sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};


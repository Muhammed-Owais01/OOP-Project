#pragma once
class Tile
{
private:
	// Basic tile on which texture is loaded
	sf::RectangleShape tile;
public:
	Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, sf::IntRect tex_change);
	~Tile();

	const std::string allToString() const;

	void update(sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};


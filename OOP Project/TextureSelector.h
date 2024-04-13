#pragma once

#include "Button.h"

class TextureSelector
{
private:
	sf::RectangleShape bounds;
	sf::Sprite sheet;
	sf::RectangleShape selector;
	sf::Vector2u mousePosGrid;
	sf::IntRect texRect;
	Button* hide_btn;

	bool active;
	bool hidden;
	float keyTime;
	float keyTimeMax;
	float gridSize;
public:
	TextureSelector(float x, float y, float width, float height, float gridSize, sf::Font& font, const sf::Texture& tex_sheet);
	~TextureSelector();

	const bool& getActive();
	const sf::IntRect& getTexRect();
	const bool getKeytime();

	void updateKeyTime(const float& dt);

	void update(sf::Vector2i& mousePosWindow, const float& dt);
	void render(sf::RenderTarget* target);
};


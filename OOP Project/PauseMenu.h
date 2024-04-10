#pragma once

#include "Button.h"

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menuText;

	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, Button*> buttons;
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	~PauseMenu();

	const std::map<std::string, Button*> getButtons();

	bool isButtonPressed(std::string key);
	void addButton(float y, std::string key, std::string name);
	void update(sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};


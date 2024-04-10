#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// enum button states for ease of access
enum button_states {BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE, BTN_PRESSED};

class Button
{
private:
	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	// text states color
	sf::Color textIdleColor;
	sf::Color textHoverColor;
	sf::Color textActiveColor;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

	short unsigned buttonState;
	short unsigned id;
	bool btn_pressed;
public:
	// Constructor to have initialize the button
	Button(float x, float y, float width, float height, 
		sf::Font* font, std::string text, int charSize,
		sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
		sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
		short unsigned id = 0);
	~Button();

	// Getters
	const bool isPressed() const;
	const std::string getText() const;
	const short unsigned& getId() const;

	// Setters
	void setText(const std::string text);
	void setId(const short unsigned id);
	void setCheckBoxColor(sf::Color color);

	void update(const sf::Vector2f mousePos, bool checkBox);
	void render(sf::RenderTarget* target);
};


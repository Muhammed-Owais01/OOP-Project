#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font)
	: font(font)
{
	// Background is the darkening of the whole screen when pause menu is pushed into the stack
	// So its size is set to window size, and a darkish color to indicate the game is paused
	this->background.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	// This is the container that has the Quit, Save, Load etc buttons
	this->container.setSize(sf::Vector2f(static_cast<float>(window.getSize().x / 4.f), static_cast<float>(window.getSize().y)));
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x / 2.f - this->container.getSize().x / 2.f), 0.f));

	// Init Text
	// The top text that says PAUSED
	this->menuText.setFont(font);
	this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText.setCharacterSize(40);
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + 20.f
	);
}

PauseMenu::~PauseMenu()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete it->second;
	}
}

// Getter of all map buttons
const std::map<std::string, Button*> PauseMenu::getButtons()
{
	return this->buttons;
}

// Getter for global bounds of the container
const sf::FloatRect& PauseMenu::getGlobalBounds() const
{
	return this->container.getGlobalBounds();
}

// Checking is a button is pressed
bool PauseMenu::isButtonPressed(std::string key)
{
	// key is the location name I want to access in the map, and then if the button there is pressed then return true else false
	if (this->buttons[key]->isPressed())
		return true;
	return false;
}

// A function made to add buttons easily to the map
void PauseMenu::addButton(float y, std::string key, std::string name)
{
	// Height and width of a button
	float height = 50.f;
	float width = 150.f;
	// Setting the position of the button in the container
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;
	// A location name at which to add a new button, so a location will have a name where there is such a button
	this->buttons[key] = new Button(x, y, width, height,
		&this->font, name, 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

void PauseMenu::update(sf::Vector2i& mousePosWindow)
{
	// Update all the buttons in the map
	for (auto& it : this->buttons)
	{
		it.second->update(mousePosWindow, false);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	// Draw the background, container, and menu text
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->menuText);
	// Render all the buttons by calling the render function of the buttons
	for (auto& it : this->buttons)
	{
		it.second->render(&target);
	}
}

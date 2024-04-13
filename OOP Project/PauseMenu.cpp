#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font)
	: font(font)
{
	this->background.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	this->container.setSize(sf::Vector2f(static_cast<float>(window.getSize().x / 4.f), static_cast<float>(window.getSize().y)));
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x / 2.f - this->container.getSize().x / 2.f), 0.f));

	// Init Text
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

const std::map<std::string, Button*> PauseMenu::getButtons()
{
	return this->buttons;
}

bool PauseMenu::isButtonPressed(std::string key)
{
	if (this->buttons[key]->isPressed())
		return true;
	return false;
}

void PauseMenu::addButton(float y, std::string key, std::string name)
{
	float height = 50.f;
	float width = 150.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;
	this->buttons[key] = new Button(x, y, width, height,
		&this->font, name, 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

void PauseMenu::update(sf::Vector2i& mousePosWindow)
{
	for (auto& it : this->buttons)
	{
		it.second->update(mousePosWindow, false);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->menuText);
	for (auto& it : this->buttons)
	{
		it.second->render(&target);
	}
}

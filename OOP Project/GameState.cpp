#include "stdafx.h"
#include "GameState.h"

void GameState::initVariables()
{
	// Initialize the player
	this->player = Player(*this->window);
}

void GameState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::GAMESTATE::COULD NOT LOAD FONT");
}

void GameState::initPauseMenu()
{
	this->pMenu = new PauseMenu(*this->window, font);

	this->pMenu->addButton(800, "QUIT", "Quit");
}

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states)
{
	this->initVariables();
	this->initFont();
	this->initPauseMenu();
}

GameState::~GameState()
{

}

void GameState::endState()
{
	std::cout << "Ending Game State\n";
}

void GameState::updateKeybinds(const float& dt)
{
	
}

void GameState::updatePause(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeyTime() == true)
	{
		std::cout << "Key Time: " << keyTime << std::endl;
		if (!this->pause)
			this->pauseState();
		else
			this->unPauseState();
	}
}

void GameState::isPausedMenuButtonsPressed()
{
	if (this->pMenu->isButtonPressed("QUIT"))
		this->quit = true;
}

void GameState::update(const float& dt)
{
	this->updateKeybinds(dt);
	this->updateMousePositions();
	this->updateKeyTime(dt);
	this->updatePause(dt);

	if (!this->pause)
	{
		this->player.update(*this->window);

		this->enemy.update(*this->window);
	}
	else
	{
		this->pMenu->update(this->mousePosView);
		this->isPausedMenuButtonsPressed();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	this->player.render(this->window);

	this->enemy.render(this->window);

	if (this->pause)
	{
		this->pMenu->render(*target);
	}
}

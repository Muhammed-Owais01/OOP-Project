#include "stdafx.h"
#include "GameState.h"

void GameState::initVariables()
{
	// Initialize the player
	this->player = Player(*this->window);
}

void GameState::initView()
{
	this->playerCamera.setSize(sf::Vector2f(this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height));
	this->playerCamera.setCenter(sf::Vector2f(
		this->stateData->settings->windowBounds.width / 2.f,
		this->stateData->settings->windowBounds.height / 2.f
	));
}

void GameState::initMap()
{
	this->tileMap = new TileMap(this->stateData->grideSize, this->stateData->grideSize, this->stateData->grideSize, "Textures/Map/grass.jpg");
	this->tileMap->loadFromFile("Saves/data.pgsd");
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

GameState::GameState(StateData* stateData)
	: State(stateData)
{
	this->initVariables();
	this->initView();
	this->initFont();
	this->initMap();
	this->initPauseMenu();
}

GameState::~GameState()
{
	delete this->pMenu;
}

void GameState::endState()
{
	std::cout << "Ending Game State\n";
}

void GameState::updateKeybinds(const float& dt)
{
	
}

void GameState::updateView()
{
	this->playerCamera.setCenter(this->player.getPosition());
}

void GameState::updatePause(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeyTime() == true)
	{
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
	this->updateView();
	this->updateMousePositions(&this->playerCamera);
	this->updateKeyTime(dt);
	this->updatePause(dt);
	this->tileMap->update(this->mousePosView);

	if (!this->pause)
	{
		this->player.update(*this->window);

		this->enemy.update(*this->window);
	}
	else
	{
		this->pMenu->update(this->mousePosWindow);
		this->isPausedMenuButtonsPressed();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	target->setView(this->playerCamera);
	this->tileMap->render(*target);

	this->player.render(this->window);

	this->enemy.render(this->window);

	target->setView(this->window->getDefaultView());
	if (this->pause)
	{
		this->pMenu->render(*target);
	}
}

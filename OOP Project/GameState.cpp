#include "stdafx.h"
#include "GameState.h"

void GameState::initDeferredRender()
{
	this->renderTexture.create(this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height);
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(0, 0, this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height));
}

void GameState::initVariables()
{
	// Initialize the player
	this->player = new Player(*this->window);
	this->enemy = new Enemy(*this->window);
}

void GameState::initView()
{
	// Set the view to the screen size
	this->playerCamera.setSize(sf::Vector2f(this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height));
	// Center it to window center
	this->playerCamera.setCenter(sf::Vector2f(
		this->stateData->settings->windowBounds.width / 2.f,
		this->stateData->settings->windowBounds.height / 2.f
	));
	//this->playerCamera.zoom(0.4f);

	this->viewGridPos.x = static_cast<int>(this->playerCamera.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPos.y = static_cast<int>(this->playerCamera.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::initMap()
{
	// Initialize the map 
	this->tileMap = new TileMap(this->stateData->gridSize, /*this->stateData->gridSize*/ 100.f, /*this->stateData->gridSize*/ 100.f, "Textures/Map/tileMap.png");
	// Load the map
	this->tileMap->loadFromFile("Saves/data.pgsd");
}

void GameState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::GAMESTATE::COULD NOT LOAD FONT");
}

void GameState::initPauseMenu()
{
	// Initialize the pause menu
	this->pMenu = new PauseMenu(*this->window, font);

	// Add the quit button
	this->pMenu->addButton(200, "QUIT", "Quit");
}

GameState::GameState(StateData* stateData)
	: State(stateData)
{
	// Call all the init functions
	this->initDeferredRender();
	this->initVariables();
	this->initView();
	this->initFont();
	this->initMap();
	this->initPauseMenu();
}

GameState::~GameState()
{
	delete this->pMenu;
	delete this->tileMap;
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
	// Set the camera to player center
	this->playerCamera.setCenter(this->player->getPosition());

	this->viewGridPos.x = static_cast<int>(this->playerCamera.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPos.y = static_cast<int>(this->playerCamera.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::updatePause(const float& dt)
{
	// If escape is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeyTime() == true)
	{
		// Then if not pause then pause
		if (!this->pause)
			this->pauseState();
		else
			this->unPauseState();
	}
}

void GameState::isPausedMenuButtonsPressed()
{
	// If quit is pressed, then set quit to true
	if (this->pMenu->isButtonPressed("QUIT"))
		this->quit = true;
}

void GameState::update(const float& dt)
{
	// Call all the update functions
	this->updateKeybinds(dt);
	this->updateView();
	// Update mouse positions with view
	this->updateMousePositions(&this->playerCamera);
	this->updateKeyTime(dt);
	this->updatePause(dt);
	// Update the tile map
	this->tileMap->update(this->mousePosView, this->player, this->viewGridPos);

	// If not paused, then update the player and enemy 
	if (!this->pause)
	{
		this->player->update(*this->window);

		this->enemy->update(*this->window);
	}
	// Update pause menu
	else
	{
		this->pMenu->update(this->mousePosWindow);
		this->isPausedMenuButtonsPressed();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	// Render map, player, enemy in the view
	this->renderTexture.clear();

	this->renderTexture.setView(this->playerCamera);
	this->tileMap->render(this->renderTexture, this->viewGridPos);

	this->player->render(&this->renderTexture);

	//this->enemy->render(&this->renderTexture);

	// Render pause menu in window
	
	if (this->pause)
	{
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pMenu->render(this->renderTexture);
	}
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}

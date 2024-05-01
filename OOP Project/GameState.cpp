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
}

void GameState::initBackground()
{
	if (!this->back_tex.loadFromFile("Textures/Backgrounds/GameBackground.png"))
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD GameBackground";
	this->background.setSize(static_cast<sf::Vector2f>(this->window->getSize()));
	this->background.setPosition(sf::Vector2f(0.f, 100.f));
	this->background.setTexture(&this->back_tex);
}

void GameState::initView()
{
	// Set the view to the screen size
	this->playerCamera.setSize(sf::Vector2f(static_cast<float>(this->stateData->settings->windowBounds.width), static_cast<float>(this->stateData->settings->windowBounds.height)));
	// Center it to window center
	this->playerCamera.setCenter(sf::Vector2f(
		this->stateData->settings->windowBounds.width / 2.f,
		this->stateData->settings->windowBounds.height / 2.f
	));
	this->playerCamera.zoom(0.4f);

	this->viewGridPos.x = static_cast<int>(this->playerCamera.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPos.y = static_cast<int>(this->playerCamera.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::initMap()
{
	// Initialize the map 
	this->tileMap = new TileMap(this->stateData->gridSize, 1000, 100, "Textures/Map/tileMap.png");
	// Load the map
	this->tileMap->loadFromFile("Saves/data.pgsd");
}

void GameState::initEnemies()
{
	// Initialize the enemy list
	this->enemy_list = new Enemies(this->stateData->gridSize, 1000, 100);
	// Load all the enemies
	this->enemy_list->loadFromFile("Saves/enemySpawnData.pgsd");
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
	this->initBackground();
	this->initMap();
	this->initEnemies();
	this->initPauseMenu();
}

GameState::~GameState()
{
	delete this->pMenu;
	delete this->tileMap;
	delete this->enemy_list;
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
	if (this->pMenu->isButtonPressed("QUIT") || !player->getHp())
		this->quit = true;
}

void GameState::update(const float& dt)
{
	if (!player->getHp())
		this->quit = true;

	// Call all the update functions
	this->updateKeybinds(dt);
	this->updateView();
	// Update mouse positions with view
	this->updateMousePositions(&this->playerCamera);
	this->updateKeyTime(dt);
	this->updatePause(dt);
	// Update the tile map
	this->tileMap->update(this->mousePosView, this->player, this->viewGridPos, dt);
	// Update the enemy list
	this->enemy_list->update(*window, this->viewGridPos, dt, player);

	// If not paused, then update the player and enemy 
	if (!this->pause)
	{
		this->player->update(*this->window);
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
	this->renderTexture.draw(this->background);
	this->tileMap->render(this->renderTexture, this->viewGridPos);
	this->enemy_list->render(&this->renderTexture, this->viewGridPos);

	this->player->render(&this->renderTexture);

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

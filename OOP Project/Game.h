#pragma once

#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "MainMenuState.h"

class Game
{
private:
	// Window settings variable
	sf::RenderWindow* window;
	sf::Event event;
	std::vector<sf::VideoMode> videoModes;
	sf::ContextSettings windowSettings;
	bool fullscreen;

	// dt variables
	sf::Clock dtClock;
	float dt;

	// State pointer stack
	std::stack<State*> states;

	sf::View view;
	TileMap map;

	float viewSpeed;

	// init Methods
	void initWindow();
	void initStates();
	void initVariables();
	void initMap();
public:
	Game();
	~Game();

	void moveView();
	
	void endApplication();

	// Update Methods
	void updatePollEvents();
	void updateDT();
	void update();

	// Render Methods
	void render();

	void run();
};


#pragma once

#include "Player.h"
#include "Enemy.h"
#include "MainMenuState.h"
#include "WindowSettings.h"
#include "State.h"

class Game
{
private:
	StateData stateData;
	// Window settings variable
	sf::RenderWindow* window;
	sf::Event event;
	
	WindowSettings settings;

	// dt variables
	sf::Clock dtClock;
	float dt;

	// State pointer stack
	std::stack<State*> states;

	sf::View view;

	float viewSpeed;
	float grideSize;

	// init Methods
	void initWindow();
	void initStates();
	void initVariables();
	void initStateData();
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


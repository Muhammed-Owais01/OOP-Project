#pragma once

#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "MainMenuState.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	sf::View view;
	TileMap map;

	float viewSpeed;

	void initWindow();
	void initStates();
	void initVariables();
	void initMap();
public:
	Game();
	~Game();

	void moveView();
	
	void endApplication();

	void updatePollEvents();
	void updateDT();
	void update();
	void render();

	void run();
};


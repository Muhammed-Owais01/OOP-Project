#pragma once

#include "WindowSettings.h"

// Need to forward declare state because it is used in StateData
class State;
class StateData
{
public:
	StateData();

	// Variables
	float grideSize;
	sf::RenderWindow* window;
	std::stack<State*>* states;
	WindowSettings* settings;
};

class State
{
protected:
	// Creating one here, so all inherited classes can use it directly instead of making a new one for themselves
	StateData* stateData;
	// Pointer to a state stack that points to another stack
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::vector<sf::Texture*> texture;

	// State variables
	bool quit;
	bool pause;
	float keyTime;
	float keyTimeMax;
	float grideSize;

	// Store mouse locations
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;

public:
	State(StateData* stateData);
	~State();

	// Accessors
	const bool& getQuit() const;
	const bool getKeyTime();

	// Functions
	void pauseState();
	void unPauseState();
	void updateKeyTime(const float& dt);

	// Pure Virtual Functions
	virtual void endState() = 0;
	virtual void updateMousePositions(sf::View* view = nullptr);
	virtual void updateKeybinds(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};


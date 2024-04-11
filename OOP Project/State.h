#pragma once

class State
{
protected:
	// Pointer to a state stack that points to another stack
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::vector<sf::Texture*> texture;

	// State variables
	bool quit;
	bool pause;
	float keyTime;
	float keyTimeMax;

	// Store mouse locations
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

public:
	State(sf::RenderWindow* window, std::stack<State*>* states);
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
	virtual void updateMousePositions();
	virtual void updateKeybinds(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};


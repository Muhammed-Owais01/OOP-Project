#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <tuple>
#include <stack>
#include <map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class State
{
protected:
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::vector<sf::Texture*> texture;
	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

public:
	State(sf::RenderWindow* window, std::stack<State*>* states);
	~State();

	const bool& getQuit() const;
	
	virtual void endState() = 0;
	virtual void updateMousePositions();
	virtual void updateKeybinds(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};


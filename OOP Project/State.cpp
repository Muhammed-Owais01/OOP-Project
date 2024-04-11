#include "stdafx.h"
#include "State.h"

State::State(sf::RenderWindow* window, std::stack<State*>* states)
{
	this->window = window;
	// Point the states stack to another stack
	this->states = states;
	this->quit = false;
	this->pause = false;
	this->keyTime = 0.f;
	this->keyTimeMax = 10000.f;
}

State::~State()
{

}

// getter for quit
const bool& State::getQuit() const
{
	return this->quit;
}

// Getter of keyTime
const bool State::getKeyTime()
{
	// reset keyTime to 0 if it is greater than max
	if (this->keyTime > this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

void State::pauseState()
{
	this->pause = true;
}

void State::unPauseState()
{
	this->pause = false;
}

// Update KeyTime Value
void State::updateKeyTime(const float& dt)
{
	// add to keyTime if it is less than max, keyTime is to have a duration between state switching from pause to unpause
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
	}
}

// Update the mousePosition variables as the mouse position changes
void State::updateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

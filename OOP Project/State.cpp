#include "stdafx.h"
#include "State.h"

State::State(StateData* stateData)
{
	this->stateData = stateData;
	this->window = stateData->window;
	// Point the states stack to another stack
	this->states = stateData->states;
	this->quit = false;
	this->pause = false;
	this->keyTime = 0.f;
	this->keyTimeMax = 5000.f;
	this->grideSize = stateData->grideSize;
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
void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if (view)
		this->window->setView(*view);

	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
	this->mousePosGrid = sf::Vector2u(
		static_cast<unsigned>(this->mousePosView.x) / static_cast<unsigned>(this->grideSize),
		static_cast<unsigned>(this->mousePosView.y) / static_cast<unsigned>(this->grideSize)
	);

	this->window->setView(this->window->getDefaultView());
}

StateData::StateData()
{
}

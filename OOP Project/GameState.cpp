#include "GameState.h"

void GameState::initVariables()
{
	this->player = Player(*this->window);
}

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states)
{
	this->initVariables();
}

GameState::~GameState()
{

}

void GameState::endState()
{
	std::cout << "Ending Game State\n";
}

void GameState::updateKeybinds(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		this->quit = true;
}

void GameState::update(const float& dt)
{
	this->updateKeybinds(dt);

	this->player.update(*this->window);

	this->enemy.update(*this->window);
}

void GameState::render(sf::RenderTarget* target)
{
	this->player.render(this->window);

	this->enemy.render(this->window);
}

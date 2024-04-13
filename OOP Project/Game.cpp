#include "stdafx.h"
#include "Game.h"

void Game::initWindow()
{
	settings.loadFromFile("Config/window.ini");

	// Setting fullscreen
	if (this->settings.fullscreen)
		this->window = new sf::RenderWindow(this->settings.windowBounds, this->settings.title, sf::Style::Fullscreen, this->settings.windowSettings);
	else
		this->window = new sf::RenderWindow(this->settings.windowBounds, this->settings.title, sf::Style::Titlebar | sf::Style::Close, this->settings.windowSettings);

	this->window->setFramerateLimit(this->settings.frameLimit);
	this->window->setVerticalSyncEnabled(this->settings.vertical_sync_enabled);

	this->view.setSize(static_cast<float>(this->settings.windowBounds.width), static_cast<float>(this->settings.windowBounds.height));
	this->view.setCenter(sf::Vector2f(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f));
}

// Initialize States
void Game::initStates()
{
	// Push MainMenuState as the first state of the game to show main menu
	this->states.push(new MainMenuState(&this->stateData));
}

// Initialize Variables
void Game::initVariables()
{
	this->dt = 0.f;
	this->viewSpeed = 10.f;
	this->grideSize = 100.f;
}

void Game::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.settings = &this->settings;
	this->stateData.states = &this->states;
	this->stateData.grideSize = this->grideSize;
}

// Call Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initStateData();
	this->initStates();
}

// Delete the dynamically allocated variables
Game::~Game()
{
	delete this->window;

	// If states stack is not empty then delete the pointer to it, and pop it
	if (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

void Game::moveView()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->view.move(sf::Vector2f(0.f, -viewSpeed));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->view.move(sf::Vector2f(0.f, viewSpeed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->view.move(sf::Vector2f(-viewSpeed, 0.f));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->view.move(sf::Vector2f(viewSpeed, 0.f));
}

// Execute when ending application
void Game::endApplication()
{
	std::cout << "Ending Application\n";
}

void Game::updatePollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed)
			this->window->close();
	}
}

// Delta Time Update
void Game::updateDT()
{
	this->dt = this->dtClock.getElapsedTime().asSeconds();
}

// Update of Game(here all update methods are called)
void Game::update()
{
	this->updateDT();
	this->updatePollEvents();

	// If the states stack isnt empty, update the state at the top
	if (!this->states.empty())
	{
		// Call the update of top state of stack
		this->states.top()->update(this->dt);
		// See if top of stack is to quit
		if (this->states.top()->getQuit())
		{
			// Call endState of top of stack state
			this->states.top()->endState();
			// Delete pointer to it
			delete this->states.top();
			// Pop the top state
			this->states.pop();
		}
	}
	// If stack emtpy then close window
	else
	{
		this->endApplication(); 
		this->window->close();
	}
	//this->moveView();
}

// Game render method(all the render methods are called here)
void Game::render()
{
	this->window->clear();

	// If stack isnt empty then render the top of stack state
	if (!this->states.empty())
		this->states.top()->render(this->window);

	// Render in View
	this->window->setView(this->view);

	// Render UI
	this->window->setView(this->window->getDefaultView());

	this->window->display();
}


// The method where update and render is called
void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

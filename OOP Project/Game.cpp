#include "Game.h"

void Game::initWindow()
{
	// To pick config data from window.ini file to set window settings
	std::ifstream ifs("Config/window.ini");

	// Variables to store window settings
	this->videoModes = sf::VideoMode::getFullscreenModes();
	std::string title = "None";
	sf::VideoMode windowBounds = sf::VideoMode::getDesktopMode();
	unsigned frameLimit = 120;
	bool vertical_sync_enabled = false;
	unsigned antialiasing_level = 0;

	// While file is open
	if (ifs.is_open())
	{
		// Extract title
		getline(ifs, title);
		// Extract width height full screen frameLimit, vertical sync, antialising level
		ifs >> windowBounds.width >> windowBounds.height;
		ifs >> this->fullscreen;
		ifs >> frameLimit;
		ifs >> vertical_sync_enabled;
		ifs >> antialiasing_level;
	}
	// Close file
	ifs.close();

	this->windowSettings.antialiasingLevel = antialiasing_level;

	// Setting fullscreen
	if (this->fullscreen)
		this->window = new sf::RenderWindow(windowBounds, title, sf::Style::Fullscreen, this->windowSettings);
	else
		this->window = new sf::RenderWindow(windowBounds, title, sf::Style::Titlebar | sf::Style::Close, this->windowSettings);

	this->window->setFramerateLimit(frameLimit);
	this->window->setVerticalSyncEnabled(vertical_sync_enabled);

	this->view.setSize(static_cast<float>(windowBounds.width), static_cast<float>(windowBounds.height));
	this->view.setCenter(sf::Vector2f(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f));
}

// Initialize States
void Game::initStates()
{
	// Push MainMenuState as the first state of the game to show main menu
	this->states.push(new MainMenuState(this->window, &this->states));
}

// Initialize Variables
void Game::initVariables()
{
	this->fullscreen = false;
	this->viewSpeed = 10.f;
}

void Game::initMap()
{
	const int level[] =
	{
		17
	};
	int num1, num2, num3;
	std::ifstream inputFile("Map/MapData.txt");
	std::vector<int> Test;

	std::vector<std::tuple<int, int, int>> tiles;
	int c = 0;
	while (inputFile.is_open())
	{
		inputFile >> num1 >> num2 >> num3;
		tiles.push_back(std::make_tuple(num1, num2, num3));
		c++;
		if (c == 6)
			break;
	}
	inputFile.close();

	if (!map.load("Textures/test.png", tiles, 1, 1))
		std::cout << "COULD NOT LOAD tileset.png";
}

// Call Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initStates();
	this->initMap();
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

	this->window->draw(this->map);

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

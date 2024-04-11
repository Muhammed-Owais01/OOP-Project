#include "stdafx.h"
#include "MainMenuState.h"

// Initialize font for menu state
void MainMenuState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
}

// Initialize background
void MainMenuState::initBackground()
{
	this->background.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));

	if (!this->bgTexture.loadFromFile("Textures/Backgrounds/bg.png"))
		throw("ERROR::MAINMENUSTATE::initBackground()::Could not load bg.png");

	this->background.setTexture(&this->bgTexture);
}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states)
{
	// Call the init functions
	this->initFont();
	this->initBackground();

	// Add a GAME_STATE button in stack
	this->buttons["GAME_STATE"] = new Button(400.f, 300.f, 150.f, 50.f,
		&this->font, "New Game", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	// Add a SETTINGS_STATE button in stack
	this->buttons["SETTINGS_STATE"] = new Button(400.f, 400.f, 150.f, 50.f,
		&this->font, "Settings", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	// Add a EDITOR_STATE button in stack
	this->buttons["EDITOR_STATE"] = new Button(400.f, 500.f, 150.f, 50.f,
		&this->font, "Editor", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	// Add a EXIT_STATE button in stack
	this->buttons["EXIT_STATE"] = new Button(400.f, 600.f, 150.f, 50.f,
		&this->font, "Exit Game", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);
}

MainMenuState::~MainMenuState()
{
	// Delete button pointers
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete it->second;
	}
}

void MainMenuState::endState()
{
	std::cout << "Ending Game State\n";
}

void MainMenuState::updateKeybinds(const float& dt)
{
}

void MainMenuState::updateButtons()
{
	// Update all the buttons in stack
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView, false);
	}

	// push GameState when it is pressed
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		this->states->push(new GameState(this->window, states));
	}

	// push EditorState when it is pressed
	if (this->buttons["EDITOR_STATE"]->isPressed())
	{
		this->states->push(new EditorState(this->window, states));
	}

	if (this->buttons["SETTINGS_STATE"]->isPressed())
	{
		this->states->push(new SettingsState(this->window, states));
	}

	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->quit = true;
	}
}

// Call all the update methods here
void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateButtons();
	this->updateKeybinds(dt);

}

void MainMenuState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	target->draw(this->background);
	this->renderButtons(target);

	/*sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << "X: " << this->mousePosView.x << " Y: " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);*/
}


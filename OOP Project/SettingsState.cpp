#include "stdafx.h"
#include "SettingsState.h"

// Initialize font for menu state
void SettingsState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::SETTINGSTATE::COULD NOT LOAD FONT");
}

// The options text
void SettingsState::initText()
{
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(200.f, 300.f));
	this->optionsText.setCharacterSize(50);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));

	this->optionsText.setString(
		"Resolution\n\nFullScreen\n\nVsync\n\nAntialiasing\n\n"
	);
}

// Creating check boxes
void SettingsState::initCheckBox()
{
	// full screen check box
	this->fullScreenCheckBox = new Button(920.f, 425.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));

	// vSync check box
	this->vSyncCheckBox = new Button(920.f, 560.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));

	// anti-aliasing check box
	this->AACheckBox = new Button(920.f, 695.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));
}

void SettingsState::initModes()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

SettingsState::SettingsState(StateData* stateData)
	: State(stateData), defaultIndex(0)
{
	// Call the init functions
	this->initFont();
	this->initText();
	this->initModes();
	this->initCheckBox();
	// Restarting both clocks
	this->applyClock.restart();
	this->settingsClock.restart();

	// Add a EXIT_STATE button in stack
	this->buttons["BACK_STATE"] = new Button(1600.f, 940.f, 150.f, 50.f,
		&this->font, "Back", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	// Add Apply state button
	this->buttons["APPLY_STATE"] = new Button(1400.f, 940.f, 150.f, 50.f,
		&this->font, "Apply", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	// Convert the resolutions in the modes to string with an x between them and storing in modes_str vector
	for (auto& i : this->modes)
	{
		this->modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}

	// .data() converts vector to array
	this->dropDownList["RESOLUTION"] = new DropDownList(this->font, 800, 300, 300, 50, this->modes_str.size(), this->modes_str.data(), defaultIndex);
}

SettingsState::~SettingsState()
{
	// Delete button pointers
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete it->second;
	}
	delete dropDownList["RESOLUTION"];

	delete fullScreenCheckBox;
	delete vSyncCheckBox;
	delete AACheckBox;
}

void SettingsState::endState()
{
	std::cout << "Ending Game State\n";
}

// Conversion from string to vector2u of resolution
sf::Vector2u SettingsState::stringToVector2u(const std::string& str)
{
	std::stringstream ss(str);
	unsigned int width, height;
	char delimiter;
	if (ss >> width >> delimiter >> height && delimiter == 'x') {
		return sf::Vector2u(width, height);
	}
	else {
		std::cout << "Invalid format for string to Vector2u conversion" << std::endl;
		return sf::Vector2u(0, 0); // Return default value or handle error
	}
}

void SettingsState::updateKeybinds(const float& dt)
{
}

void SettingsState::updateButtons(const float& dt)
{
	stateData->settings->loadFromFile("Config/window.ini");
	// Update all the buttons in stack
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow, false);
	}

	for (auto& it : this->dropDownList)
	{
		it.second->update(this->mousePosWindow, dt);
	}
	this->fullScreenCheckBox->update(this->mousePosWindow, true);
	this->vSyncCheckBox->update(this->mousePosWindow, true);
	this->AACheckBox->update(this->mousePosWindow, true);

	// Pressing the check box
	if (this->settingsClock.getElapsedTime().asSeconds() >= 5.f)
	{
		if (this->fullScreenCheckBox->isPressed())
			(stateData->settings->fullscreen == 0) ? (stateData->settings->fullscreen = 1) : (stateData->settings->fullscreen = 0);

		if (this->vSyncCheckBox->isPressed())
			(stateData->settings->vertical_sync_enabled == false) ? (stateData->settings->vertical_sync_enabled = true) : (stateData->settings->vertical_sync_enabled = false);

		if (this->AACheckBox->isPressed())
			(stateData->settings->windowSettings.antialiasingLevel == 0) ? (stateData->settings->windowSettings.antialiasingLevel = 1) : (stateData->settings->windowSettings.antialiasingLevel = 0);
	}

	// Pressing Apply Button
	if ((this->buttons["APPLY_STATE"]->isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) && this->applyClock.getElapsedTime().asSeconds() >= 1.f)
	{
		this->btn_sound.play();
		// Restart the clock when apply state is clicked
		this->applyClock.restart();

		// Changing window settings
		std::string str_res = this->modes_str[this->dropDownList["RESOLUTION"]->getActiveElementId()];
		sf::Vector2u resSize = stringToVector2u(str_res);
		this->window->setSize(resSize);
		if (stateData->settings->fullscreen == 1)
			this->window->create(this->modes[this->dropDownList["RESOLUTION"]->getActiveElementId()], "RPG GAME", sf::Style::Fullscreen, stateData->settings->windowSettings);
		else
			this->window->create(this->modes[this->dropDownList["RESOLUTION"]->getActiveElementId()], "RPG GAME", sf::Style::Default, stateData->settings->windowSettings);

		// Storing active element resolution in window bounds
		stateData->settings->windowBounds = modes[this->dropDownList["RESOLUTION"]->getActiveElementId()];

		stateData->settings->saveToFile("Config/window.ini");
	}

	// Back button
	if (this->buttons["BACK_STATE"]->isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->btn_sound.play();
		this->quit = true;
	}
}

// Call all the update methods here
void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateButtons(dt);
	this->updateKeybinds(dt);

}

void SettingsState::renderButtons(sf::RenderTarget* target)
{
	// Render all buttons, have check box on top, so that red doesnt go over drop down menu
	this->fullScreenCheckBox->render(target);
	this->vSyncCheckBox->render(target);
	this->AACheckBox->render(target);
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

	for (auto& it : this->dropDownList)
	{
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	target->draw(this->optionsText);
	this->renderButtons(target);

	/*sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << "X: " << this->mousePosView.x << " Y: " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText); 
	target->draw(this->optionsText);*/
}

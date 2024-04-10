#include "SettingsState.h"

// Initialize font for menu state
void SettingsState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::SETTINGSTATE::COULD NOT LOAD FONT");
}

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

void SettingsState::initCheckBox()
{
	this->fullScreenCheckBox = new Button(920.f, 425.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));
	this->vSyncCheckBox = new Button(920.f, 560.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));
	this->AACheckBox = new Button(920.f, 695.f, 50, 50,
		&this->font, "", 12,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200));
}

void SettingsState::initModes()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

SettingsState::SettingsState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states), defaultIndex(0)
{
	// Call the init functions
	this->initFont();
	this->initText();
	this->initModes();
	this->initCheckBox();
	this->applyClock.restart();
	this->settingsClock.restart();

	// Add a EXIT_STATE button in stack
	this->buttons["BACK_STATE"] = new Button(1600.f, 940.f, 150.f, 50.f,
		&this->font, "Back", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

	this->buttons["APPLY_STATE"] = new Button(1400.f, 940.f, 150.f, 50.f,
		&this->font, "Apply", 50,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0)
	);

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
}

void SettingsState::endState()
{
	std::cout << "Ending Game State\n";
}

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
	// Variables to store window settings
	std::string title = "None";
	sf::VideoMode windowBounds = sf::VideoMode::getDesktopMode();
	unsigned frameLimit = 120;
	bool vertical_sync_enabled = false;
	unsigned antialiasing_level = 0;
	bool fullscreen = 0;

	std::ifstream ifs("Config/window.ini");

	// While file is open
	if (ifs.is_open())
	{
		// Extract title
		getline(ifs, title);
		// Extract width height full screen frameLimit, vertical sync, antialising level
		ifs >> windowBounds.width >> windowBounds.height;
		ifs >> fullscreen;
		ifs >> frameLimit;
		ifs >> vertical_sync_enabled;
		ifs >> antialiasing_level;
	}
	ifs.close();

	// Update all the buttons in stack
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView, false);
	}

	for (auto& it : this->dropDownList)
	{
		it.second->update(this->mousePosView, dt);
	}
	this->fullScreenCheckBox->update(this->mousePosView, true);
	this->vSyncCheckBox->update(this->mousePosView, true);
	this->AACheckBox->update(this->mousePosView, true);

	if (this->settingsClock.getElapsedTime().asSeconds() >= 5.f)
	{
		if (this->fullScreenCheckBox->isPressed())
			(fullscreen == 0) ? (fullscreen = 1) : (fullscreen = 0);

		if (this->vSyncCheckBox->isPressed())
			(vertical_sync_enabled == false) ? (vertical_sync_enabled = true) : (vertical_sync_enabled = false);

		if (this->AACheckBox->isPressed())
			(antialiasing_level == 0) ? (antialiasing_level = 1) : (antialiasing_level = 0);
	}

	if ((this->buttons["APPLY_STATE"]->isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) && this->applyClock.getElapsedTime().asSeconds() >= 1.f)
	{
		this->applyClock.restart();
		std::string str_res = this->modes_str[this->dropDownList["RESOLUTION"]->getActiveElementId()];
		sf::Vector2u resSize = stringToVector2u(str_res);
		this->window->setSize(resSize);
		if (fullscreen == 1)
			this->window->create(this->modes[this->dropDownList["RESOLUTION"]->getActiveElementId()], "RPG GAME", sf::Style::Fullscreen);
		else
			this->window->create(this->modes[this->dropDownList["RESOLUTION"]->getActiveElementId()], "RPG GAME", sf::Style::Default);
		this->window->setVerticalSyncEnabled(vertical_sync_enabled);

		windowBounds = modes[this->dropDownList["RESOLUTION"]->getActiveElementId()];

		std::ofstream fout("Config/window.ini");
		fout << title << std::endl 
			<< windowBounds.width << " " << windowBounds.height << std::endl 
			<< fullscreen << std::endl 
			<< frameLimit << std::endl
			<< vertical_sync_enabled << std::endl
			<< antialiasing_level << std::endl;

		fout.close();
	}

	if (this->buttons["BACK_STATE"]->isPressed())
	{
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
	this->renderButtons(target);

	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << "X: " << this->mousePosView.x << " Y: " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);
	target->draw(this->optionsText);
}

#include "stdafx.h"
#include "EditorState.h"

// Initialize font for menu state
void EditorState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
}

void EditorState::initView()
{
	// Set the view size to window size
	this->view.setSize(sf::Vector2f(this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height));
	// Set the view center to window center
	this->view.setCenter(sf::Vector2f(
		this->stateData->settings->windowBounds.width / 2.f,
		this->stateData->settings->windowBounds.height / 2.f
	));
	this->view.zoom(0.4f);
}

void EditorState::initVariables()
{
	this->viewSpeed = 0.4f;
	this->collision = false;
	this->type = 0;
	this->viewGridPos.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPos.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void EditorState::initPauseMenu()
{
	// Initialize menu
	this->pMenu = new PauseMenu(*this->window, font);

	// Add Quit, Save, Load buttons to pause menu
	this->pMenu->addButton(200.f, "QUIT", "Quit");
	this->pMenu->addButton(500.f, "SAVE", "Save");
	this->pMenu->addButton(300.f, "LOAD", "Load");
}

void EditorState::initSelector()
{
	// Side of side bar in height is the whole window height
	this->sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(this->stateData->settings->windowBounds.height)));
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	this->sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
	this->sidebar.setOutlineThickness(1.f);

	// The selector to add map in the editor(the green box)
	// Set the box size to that of hte grid size
	this->rectSelector.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));
	this->rectSelector.setOutlineColor(sf::Color::Green);
	this->rectSelector.setOutlineThickness(2.f);
	this->rectSelector.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
	this->rectSelector.setFillColor(sf::Color(255, 255, 255, 120));

	// Tex rectangle of size grid
	this->texRect = sf::IntRect(0, 0, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));

	// Set texture of selector
	this->rectSelector.setTexture(&this->map->getTileTex());
	// Set the texture in this selector to have the textRect
	this->rectSelector.setTextureRect(this->texRect);

	// Texture selector on top left initialize x 20 and y 20, size 500 width height
	this->texSelector = new TextureSelector(20.f, 20.f, 500.f, 500.f, this->stateData->gridSize, this->font, this->map->getTileTex());
}

void EditorState::initText()
{
	// Set to mouse position in the view
	this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	this->cursorText.setFont(this->font);
	this->cursorText.setCharacterSize(12);
}

void EditorState::initMap()
{
	// Initialize tile map
	this->map = new TileMap(this->stateData->gridSize, 1000.f, 100.f, "Textures/Map/tileMap.png");
}

void EditorState::initEnemyList()
{
	// Initialize the enemy list
	this->enemy_list = new Enemies(this->stateData->gridSize, 1000.f, 100.f);
}

EditorState::EditorState(StateData* stateData)
	: State(stateData)
{
	// Call initialize functions in correct
	this->initView();
	this->initVariables();
	this->initFont();
	this->initPauseMenu();
	this->initMap();
	this->initEnemyList();
	this->initSelector();
	this->initText();
}

EditorState::~EditorState()
{
	// Delete button pointers
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete it->second;
	}

	// Delete pause menu pointer
	delete this->pMenu;

	delete this->map;
	delete this->texSelector;

	delete this->enemy_list;
}

// Testing purposes
void EditorState::endState()
{
	std::cout << "Ending Game State\n";
}

// Testing maybe might use later
void EditorState::updateKeybinds(const float& dt)
{
}

void EditorState::updateView(const float& dt)
{
	// To move the view
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->view.move(this->viewSpeed * dt, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->view.move(0.f, this->viewSpeed * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->view.move(0.f, -this->viewSpeed * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->view.move(-this->viewSpeed * dt, 0.f);

	// Update the view position, according to grid, set the x and y divided by grid size to get view grid position
	this->viewGridPos.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPos.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void EditorState::updateButtons()
{
	// Update all the buttons in the map
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow, false);
	}
}

void EditorState::updatePause(const float& dt)
{
	// If escape is pressed then pause
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeyTime() == true)
	{
		if (!this->pause)
			this->pauseState();
		else
			this->unPauseState();
	}
}

void EditorState::isPausedMenuButtonsPressed()
{
	// Set quit to true if the quit button is pressed
	if (this->pMenu->isButtonPressed("QUIT"))
		this->quit = true;

	// Call save to file function if save button is pressed
	if (this->pMenu->isButtonPressed("SAVE"))
	{
		this->map->saveToFile("Saves/data.pgsd");
		this->enemy_list->saveToFile("Saves/enemySpawnData.pgsd");
	}

	// Call load from file function if load button is pressed
	if (this->pMenu->isButtonPressed("LOAD"))
	{
		this->map->loadFromFile("Saves/data.pgsd");
		this->enemy_list->loadFromFile("Saves/enemySpawnData.pgsd");
	}
}

void EditorState::updateCursorText()
{
	// Cursor text for debugging
	std::stringstream ss;
	ss << "View X: " << this->mousePosView.x << " View Y: " << this->mousePosView.y << std::endl 
		<< "Grid X: " << this->mousePosGrid.x << " Grid Y: " << this->mousePosGrid.y << std::endl
		<< "Collision: " << this->collision << std::endl
		<< "Type: " << this->type << std::endl;
	this->cursorText.setString(ss.str());
	// Set the text near the cursor of the mouse
	this->cursorText.setPosition(this->mousePosView.x + 50, this->mousePosView.y - 50);
}

void EditorState::updateEditorInput()
{
	// If C is pressed then turn on or off the collision
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		if (this->collision)
			this->collision = false;
		else
			this->collision = true;
	}

	// Up/Down in keyboard to increase/decrease type
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->getKeyTime())
		this->type++;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && this->getKeyTime())
		this->type--;

	// Left mouse button pressed for adding of tile to map
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime())
	{
		// If mouse is not inside the side bar, then we can add a map tile
		if (!this->sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->mousePosWindow)))
		{
			// If the texSelector is not active then add the tile to the map through the addToMap function
			if (!this->texSelector->getActive())
			{
				map->addToMap(this->mousePosGrid.x, this->mousePosGrid.y, 0, texRect, this->collision, this->type);
			}
			else
				// Set texture rectangle to the texture rectangle from the texture selector
				this->texRect = this->texSelector->getTexRect();
		
		}
	}

	// If right click then remove the tile from the map
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeyTime())
		map->removeFromMap(this->mousePosGrid.x, this->mousePosGrid.y, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->getKeyTime())
		this->enemy_list->addEnemy(this->mousePosGrid.x, this->mousePosGrid.y, 0, ENEMY_TYPE::BOAR);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && this->getKeyTime())
		this->enemy_list->removeEnemy(this->mousePosGrid.x, this->mousePosGrid.y, 0);
}

void EditorState::updateGUI(const float& dt)
{
	// Update the texture selector
	this->texSelector->update(this->mousePosWindow, dt);

	// Update the rectangle selector with the current texture rectangle 
	this->rectSelector.setTextureRect(this->texRect);
	this->rectSelector.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
}

void EditorState::update(const float& dt)
{
	// Call all the update functions in the correct order
	this->updateMousePositions(&this->view);
	this->updateButtons();
	this->updateKeyTime(dt);
	this->updatePause(dt);

	// If not paused then call the following update functions
	if (!this->pause)
	{
		// Update the view if not paused(allow us to move the view)
		this->updateView(dt);
		// Update the cursor text with the values
		this->updateCursorText();
		// Update the input of the editor(add/remove tiles)
		this->updateEditorInput();
		// Update the map(collision etc)
		this->map->update(this->mousePosView, nullptr, this->viewGridPos, dt);
		// Update the GUI(texture and rectangle selector)
		this->updateGUI(dt);
		// Update the enemy list
		this->enemy_list->update(*window, this->viewGridPos);
	}
	else
	{
		// Update the pause menu with the window positions
		this->pMenu->update(this->mousePosWindow);
		this->isPausedMenuButtonsPressed();
	}

}

void EditorState::renderButtons(sf::RenderTarget* target)
{
	// Render all the buttons
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::renderGUI(sf::RenderTarget* target)
{
	// Get the default view to draw sidebar on it (Interface is always drawn on default view)
	target->setView(this->window->getDefaultView());
	target->draw(this->sidebar); 

	// Set view to view  to draw the rectangle selector and cursor text in the view
	target->setView(this->view);
	if (!this->texSelector->getActive())
		target->draw(this->rectSelector);
	target->draw(this->cursorText);

	// Set it back to default view to render the texture selector on the interface
	target->setView(this->window->getDefaultView());
	this->texSelector->render(target);
}

void EditorState::render(sf::RenderTarget* target)
{
	// Set the view to view because the map will be render in the view
	target->setView(this->view);
	this->map->render(*target, this->viewGridPos);
	this->enemy_list->render(target, this->viewGridPos);

	// Set view to default view because buttons would be render in default view
	target->setView(this->window->getDefaultView());
	this->renderButtons(target);
	this->renderGUI(target);

	// Render the pause menu only if it is paused
	if (this->pause)
	{
		this->pMenu->render(*target);
	}
}

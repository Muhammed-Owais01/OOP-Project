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
	this->view.setSize(sf::Vector2f(this->stateData->settings->windowBounds.width, this->stateData->settings->windowBounds.height));
	this->view.setCenter(sf::Vector2f(
		this->stateData->settings->windowBounds.width / 2.f,
		this->stateData->settings->windowBounds.height / 2.f
	));
}

void EditorState::initVariables()
{
	this->viewSpeed = 1.f;
}

void EditorState::initPauseMenu()
{
	this->pMenu = new PauseMenu(*this->window, font);

	this->pMenu->addButton(800.f, "QUIT", "Quit");
	this->pMenu->addButton(500.f, "SAVE", "Save");
	this->pMenu->addButton(300.f, "LOAD", "Load");
}

void EditorState::initSelector()
{
	this->sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(this->stateData->settings->windowBounds.height)));
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	this->sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
	this->sidebar.setOutlineThickness(1.f);

	this->rectSelector.setSize(sf::Vector2f(this->stateData->grideSize, this->stateData->grideSize));
	this->rectSelector.setOutlineColor(sf::Color::Green);
	this->rectSelector.setOutlineThickness(2.f);
	this->rectSelector.setPosition(this->mousePosGrid.x * this->stateData->grideSize, this->mousePosGrid.y * this->stateData->grideSize);
	this->rectSelector.setFillColor(sf::Color(255, 255, 255, 120));

	this->texRect = sf::IntRect(0, 0, static_cast<int>(this->grideSize), static_cast<int>(this->grideSize));

	this->rectSelector.setTexture(&this->map->getTileTex());
	this->rectSelector.setTextureRect(this->texRect);

	this->texSelector = new TextureSelector(20.f, 20.f, 500.f, 500.f, this->stateData->grideSize, this->font, this->map->getTileTex());
}

void EditorState::initText()
{
	this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	this->cursorText.setFont(this->font);
	this->cursorText.setCharacterSize(12);
}

void EditorState::initMap()
{
	this->map = new TileMap(this->stateData->grideSize, this->stateData->grideSize, this->stateData->grideSize, "Textures/Map/grass.jpg");
}

EditorState::EditorState(StateData* stateData)
	: State(stateData)
{
	this->initVariables();
	this->initView();
	this->initFont();
	this->initPauseMenu();
	this->initMap();
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

	delete this->pMenu;
}

void EditorState::endState()
{
	std::cout << "Ending Game State\n";
}

void EditorState::updateKeybinds(const float& dt)
{
}

void EditorState::updateView(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->view.move(this->viewSpeed * dt, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->view.move(0.f, this->viewSpeed * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->view.move(0.f, -this->viewSpeed * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->view.move(-this->viewSpeed * dt, 0.f);
}

void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow, false);
	}
}

void EditorState::updatePause(const float& dt)
{

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
	if (this->pMenu->isButtonPressed("QUIT"))
		this->quit = true;

	if (this->pMenu->isButtonPressed("SAVE"))
		this->map->saveToFile("Saves/data.pgsd");

	if (this->pMenu->isButtonPressed("LOAD"))
		this->map->loadFromFile("Saves/data.pgsd");
}

void EditorState::updateCursorText()
{
	std::stringstream ss;
	ss << "View X: " << this->mousePosView.x << " View Y: " << this->mousePosView.y << std::endl 
		<< "Grid X: " << this->mousePosGrid.x << " Grid Y: " << this->mousePosGrid.y;
	this->cursorText.setString(ss.str());
	this->cursorText.setPosition(this->mousePosView.x + 50, this->mousePosView.y - 50);
}

void EditorState::updateEditorInput()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime())
	{
		if (!this->sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->mousePosWindow)))
		{
			if (!this->texSelector->getActive())
				map->addToMap(this->mousePosGrid.x, this->mousePosGrid.y, 0, texRect);
			else
				this->texRect = this->texSelector->getTexRect();
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeyTime())
		map->removeFromMap(this->mousePosGrid.x, this->mousePosGrid.y, 0);
}

void EditorState::updateGUI(const float& dt)
{
	this->texSelector->update(this->mousePosWindow, dt);

	this->rectSelector.setTextureRect(this->texRect);
	this->rectSelector.setPosition(this->mousePosGrid.x * this->stateData->grideSize, this->mousePosGrid.y * this->stateData->grideSize);

}

void EditorState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateView(dt);
	this->updateButtons();
	this->updateKeybinds(dt);
	this->updateKeyTime(dt);
	this->updatePause(dt);
	this->updateCursorText();
	this->updateEditorInput();
	this->map->update(this->mousePosView);
	this->updateGUI(dt);

	if (!this->pause)
	{
	}
	else
	{
		this->pMenu->update(this->mousePosWindow);
		this->isPausedMenuButtonsPressed();
	}

}

void EditorState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::renderGUI(sf::RenderTarget* target)
{
	target->setView(this->window->getDefaultView());
	target->draw(this->sidebar); 

	target->setView(this->view);
	if (!this->texSelector->getActive())
		target->draw(this->rectSelector);
	target->draw(this->cursorText);

	target->setView(this->window->getDefaultView());
	this->texSelector->render(target);
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->setView(this->view);
	this->map->render(*target);

	target->setView(this->window->getDefaultView());
	this->renderButtons(target);
	this->renderGUI(target);

	if (this->pause)
	{
		this->pMenu->render(*target);
	}
}

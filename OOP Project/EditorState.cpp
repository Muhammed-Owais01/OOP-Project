#include "stdafx.h"
#include "EditorState.h"

// Initialize font for menu state
void EditorState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
}

// Initialize background
void EditorState::initBackground()
{

}

EditorState::EditorState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states)
{

}

EditorState::~EditorState()
{
	// Delete button pointers
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete it->second;
	}
}

void EditorState::endState()
{
	std::cout << "Ending Game State\n";
}

void EditorState::updateKeybinds(const float& dt)
{
}

void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView, false);
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->quit = true;
	}
}

void EditorState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateButtons();
	this->updateKeybinds(dt);

}

void EditorState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::render(sf::RenderTarget* target)
{
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

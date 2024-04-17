#include "stdafx.h"
#include "DropDownList.h"

DropDownList::DropDownList(sf::Font& font, int x, int y, int width, int height, int noOfElements, std::string namesList[], int default_index)
	: font(font), keyTimeMax(1000.f)
{
	// Initialize the keytime to zero
	this->keyTime = 0.f;
	// Initialize it to false, as no list is to be shown at start
	this->showList = false;
	// Initialize the button using the constructor
	this->activeElement = new Button(x, y, width, height,
		&this->font, namesList[default_index], 18,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), 0
	);

	// Creation of all the buttons, noOfElements is the total number of buttons
	for (size_t i = 0; i < noOfElements; i++)
	{
		// Pushing back a button in the list
		this->list.push_back(
			// y + height multlied by new i, to have the button in a y below the previous one
			new Button(x, y + ((i + 1) * height), width, height,
				&this->font, namesList[i], 18,
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				i)
		);
	}
}

DropDownList::~DropDownList()
{
	delete this->activeElement;

	for (size_t i = 0; i < this->list.size(); i++)
	{
		delete this->list[i];
	}
}

const bool DropDownList::getKeytime()
{
	// If current keyTime is more than the max keytime then reset the keytime to zero and return to false
	if (this->keyTime > this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return false;
	}
	return true;
}

const unsigned short& DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

void DropDownList::updateKeyTime(const float& dt)
{
	// If keyTime is less than the max keytime then keep adding to key time
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
	}
}

void DropDownList::update(const sf::Vector2i mousePosWindow, const float& dt)
{
	// Update keytime(increase key time value if it can)
	this->updateKeyTime(dt);
	// Update the active element button
	this->activeElement->update(mousePosWindow, false);

	// If active element is pressed and keytime is true
	if (this->activeElement->isPressed() && this->getKeytime())
	{
		// Set to true if false and false if true
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList)
	{
		// Update the whole list
		for (int i = 0; i < this->list.size(); i++)
		{
			// Update each button with a false bcz it is not a checkbox
			this->list[i]->update(mousePosWindow, false);
			// If the button is pressed and keytime is true
			if (this->list[i]->isPressed() && this->getKeytime())
			{
				// Set showList to false
				this->showList = false;
				// Set active element to the element that was just clicked
				this->activeElement->setText(this->list[i]->getText());
				this->activeElement->setId(this->list[i]->getId());
			}
		}
	}
}

void DropDownList::render(sf::RenderTarget* target)
{
	this->activeElement->render(target);

	if (this->showList)
	{
		for (int i = 0; i < this->list.size(); i++)
		{
			this->list[i]->render(target);
		}
	}
}

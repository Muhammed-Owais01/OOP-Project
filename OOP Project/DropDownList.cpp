#include "stdafx.h"
#include "DropDownList.h"

DropDownList::DropDownList(sf::Font& font, int x, int y, int width, int height, int noOfElements, std::string namesList[], int default_index)
	: font(font), keyTimeMax(1000.f)
{
	this->keyTime = 0.f;
	this->showList = false;
	this->activeElement = new Button(x, y, width, height,
		&this->font, namesList[default_index], 18,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), 0
	);

	for (size_t i = 0; i < noOfElements; i++)
	{
		this->list.push_back(
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
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
	}
}

void DropDownList::update(const sf::Vector2i mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);
	this->activeElement->update(mousePosWindow, false);

	if (this->activeElement->isPressed() && this->getKeytime())
	{
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList)
	{
		for (int i = 0; i < this->list.size(); i++)
		{
			this->list[i]->update(mousePosWindow, false);
			if (this->list[i]->isPressed() && this->getKeytime())
			{
				this->showList = false;
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

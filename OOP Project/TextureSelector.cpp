#include "stdafx.h"
#include "TextureSelector.h"

TextureSelector::TextureSelector(float x, float y, float width, float height, float gridSize, sf::Font& font, const sf::Texture& tex_sheet)
	: keyTime(0.f), keyTimeMax(10.f)
{
	float offset = 60.f;

	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.setPosition(x + offset, y);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->bounds.setOutlineThickness(1.f);
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));

	this->selector.setSize(sf::Vector2f(gridSize, gridSize));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineColor(sf::Color::Red);
	this->selector.setOutlineThickness(1.f);
	this->selector.setPosition(x + offset, y);

	this->sheet.setTexture(tex_sheet);
	this->sheet.setPosition(x + offset, y);

	// Limiting the sheet to stay in the box
	if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
		this->sheet.setTextureRect(sf::IntRect(0, 0, this->bounds.getGlobalBounds().width, this->sheet.getGlobalBounds().height));
	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
		this->sheet.setTextureRect(sf::IntRect(0, 0, this->sheet.getGlobalBounds().width, this->bounds.getGlobalBounds().height));

	this->hide_btn = new Button(y, x, 50.f, 50.f,
		&font, "Hide", 16,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50)
	);

	this->active = false;
	this->gridSize = gridSize;
	this->hidden = false;

	this->texRect.width = static_cast<int>(gridSize);
	this->texRect.height = static_cast<int>(gridSize);
	
}

TextureSelector::~TextureSelector()
{
}

const bool& TextureSelector::getActive()
{
	return this->active;
}

const sf::IntRect& TextureSelector::getTexRect()
{
	return this->texRect;
}

const bool TextureSelector::getKeytime()
{
	if (this->keyTime > this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

void TextureSelector::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
	}
}

void TextureSelector::update(sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);
	this->hide_btn->update(mousePosWindow, false);
	if (this->hide_btn->isPressed() && this->getKeytime())
	{
		if (this->hidden)
			this->hidden = false;
		else
			this->hidden = true;
	}

	if (!this->hidden)
	{
		if (this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
			this->active = true;
		else
			this->active = false;

		if (this->active)
		{
			// I have to minus, if i dont then the selector position always moves to the next location before my mouse reaches there
			this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / static_cast<int>(this->gridSize);
			this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / static_cast<int>(this->gridSize);

			this->selector.setPosition(
				this->bounds.getPosition().x + this->mousePosGrid.x * this->gridSize,
				this->bounds.getPosition().y + this->mousePosGrid.y * this->gridSize
			);

			this->texRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
			this->texRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
		}
	}
}

void TextureSelector::render(sf::RenderTarget* target)
{
	if (!this->hidden)
	{
		target->draw(this->bounds);
		target->draw(this->sheet);
		if (this->active)
			target->draw(this->selector);
	}
	this->hide_btn->render(target);
}

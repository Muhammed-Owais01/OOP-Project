#include "stdafx.h"
#include "Button.h"

Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, int charSize,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
	sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
	short unsigned id) : id(id), btn_pressed(false)
{
	// Initially sets button state to idle
	this->buttonState = BTN_IDLE;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));

	// Text of buttons
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(textIdleColor);
	this->text.setCharacterSize(charSize);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
	);

	// Setting colors of text states
	this->textIdleColor = textIdleColor;
	this->textHoverColor = textHoverColor;
	this->textActiveColor = textActiveColor;

	// State Color
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;
}

Button::~Button()
{

}

// Return true if button is active
const bool Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE || this->buttonState == BTN_PRESSED)
		return true;
	return false;
}

const short unsigned& Button::getId() const
{
	return this->id;
}

const std::string Button::getText() const
{
	return this->text.getString();
}

void Button::setText(const std::string text)
{
	this->text.setString(text);
}

void Button::setId(const short unsigned id)
{
	this->id = id;
}

void Button::setCheckBoxColor(sf::Color color)
{
	this->shape.setFillColor(color);
}

void Button::update(const sf::Vector2f mousePos, bool checkBox)
{
	if (!btn_pressed)
	{
		// Idle
		this->buttonState = BTN_IDLE;
	}

	// Hover
	// If i am inside the global bounds of button, then set it to BTN_HOVER
	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		if (btn_pressed == false)
			this->buttonState = BTN_HOVER;

		// If while in its bounds, I press left mouse, make it BTN_ACTIVE
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkBox == false)
			this->buttonState = BTN_ACTIVE;
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkBox == true)
		{
			this->buttonState = BTN_PRESSED;
			if (!btn_pressed)
				btn_pressed = true;
			else
				btn_pressed = false;
		}
			
	}

	// Set text and shape color depending on button state
	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		break;
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		break;
	case BTN_PRESSED:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
		break;
	}
}

void Button::render(sf::RenderTarget* target)
{
	// Draw the shape and text
	target->draw(this->shape);
	target->draw(this->text);
}

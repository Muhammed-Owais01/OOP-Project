#include "stdafx.h"
#include "Animation.h"

void Animation::initVariables()
{
	this->animState = IDLE;
}

void Animation::initTexture(std::string path)
{
	if (!this->texSheet.loadFromFile(path))
		std::cout << "ERROR::PLAYER::Could not load player_sheet.png" << std::endl;
}

void Animation::initAnimations()
{
	this->animation_timer.restart();
	this->animationSwitch = true;
}

Animation::Animation(sf::Sprite* entity, std::string path, int width, int height)
{
	this->initVariables();
	this->initTexture(path);
	this->initAnimations();

	this->entity = entity;
	this->entity->setTexture(this->texSheet);
	this->currentFrame = sf::IntRect(0, 0, width, height); // TO GET THE SIZE, OPEN IMAGE PROPERTIES, DIVIDE WIDTH BY THE MAX SPRITE HORIZONTALLY, HEIGHT DIVIDE BY VERTICAL
	this->entity->setTextureRect(this->currentFrame);
	this->entity->setScale(1.1f, 1.1f);
}

Animation::~Animation()
{

}

const bool& Animation::getAnimSwitch()
{
	bool anim_switch = this->animationSwitch;

	if (this->animationSwitch)
		this->animationSwitch = false;

	return anim_switch;
}

const sf::Sprite* Animation::getEntity()
{
	return this->entity;
}

void Animation::resetAnimationTimer()
{
	this->animation_timer.restart();
	this->animationSwitch = true;
}

void Animation::updateAnimations(short stateType)
{
	this->animState = stateType;
	// IDLE Animations
	if (this->animState == IDLE)
	{
		if (this->animation_timer.getElapsedTime().asSeconds() >= 0.4f || this->getAnimSwitch())
		{
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 128.f;
			if (this->currentFrame.left >= 1536.f)
				this->currentFrame.left = 0;
			this->animation_timer.restart();
			this->entity->setTextureRect(this->currentFrame);
		}
	}
	else
		this->animation_timer.restart();

}

void Animation::render(sf::RenderTarget& target)
{
	target.draw(*this->entity);
}


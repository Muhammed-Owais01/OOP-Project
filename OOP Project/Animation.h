#pragma once

enum PLAYER_ANIMATION_STATE {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};
class Animation
{
private:
	sf::Sprite* entity;
	sf::Clock animation_timer;

	short animState;
	sf::IntRect currentFrame;
	bool animationSwitch;
	int width, height;

	void initVariables();
	void initAnimations();
public:
	Animation(sf::Sprite* entity, int width, int height);
	~Animation();

	const bool& getAnimSwitch();
	const sf::Sprite* getEntity();

	void resetAnimationTimer();
	void updateAnimations(short stateType);
	void update(short stateType);
	void render(sf::RenderTarget& target);
};


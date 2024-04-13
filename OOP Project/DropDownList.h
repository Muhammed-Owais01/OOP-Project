#pragma once
#include "Button.h"

class DropDownList
{
private:
	sf::Font& font;
	
	Button* activeElement;
	std::vector<Button*> list;

	float keyTime;
	float keyTimeMax;
	bool showList;
public:
	DropDownList(sf::Font& font, int x, int y, int width, int height, int noOfElements, std::string namesList[], int default_index = 0);
	~DropDownList();

	// Getters
	const bool getKeytime();
	const unsigned short& getActiveElementId() const;

	// Setters
	void updateKeyTime(const float& dt);
	
	void update(const sf::Vector2i mousePosWindow, const float& dt);
	void render(sf::RenderTarget* target);
};


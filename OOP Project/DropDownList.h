#pragma once
#include "Button.h"

class DropDownList
{
private:
	// Font for all the buttons as reference
	sf::Font& font;
	
	// Active button
	Button* activeElement;
	// List of multiple buttons
	std::vector<Button*> list;

	// Keytime so that the dropdown cannot be opened closed very fast
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


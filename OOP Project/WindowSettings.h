#pragma once
class WindowSettings
{
public:
	std::vector<sf::VideoMode> videoModes;
	std::string title;
	sf::VideoMode windowBounds;
	unsigned frameLimit;
	bool vertical_sync_enabled;
	sf::ContextSettings windowSettings;
	bool fullscreen;

	WindowSettings();

	void loadFromFile(std::string path);
	void saveToFile(std::string path);
};


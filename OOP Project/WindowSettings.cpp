#include "stdafx.h"
#include "WindowSettings.h"

WindowSettings::WindowSettings()
{
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->title = "DEFAULT";
	this->windowBounds = sf::VideoMode::getDesktopMode();
	this->frameLimit = 120;
	this->vertical_sync_enabled = false;
	this->fullscreen = false;
	this->windowSettings.antialiasingLevel = 0;
}

void WindowSettings::loadFromFile(std::string path)
{
	std::ifstream ifs(path);
	// While file is open
	if (ifs.is_open())
	{
		// Extract title
		getline(ifs, this->title);
		// Extract width height full screen frameLimit, vertical sync, antialising level
		ifs >> this->windowBounds.width >> this->windowBounds.height;
		ifs >> this->fullscreen;
		ifs >> this->frameLimit;
		ifs >> this->vertical_sync_enabled;
		ifs >> this->windowSettings.antialiasingLevel;
	}
	// Close file
	ifs.close();
}

void WindowSettings::saveToFile(std::string path)
{
	// Changing the file data with the new settings
	std::ofstream fout(path);

	if (fout.is_open())
	{
		fout << this->title << std::endl;
		fout << this->windowBounds.width << " " << this->windowBounds.height << std::endl;
		fout << this->fullscreen << std::endl;
		fout << this->frameLimit << std::endl;
		fout << this->vertical_sync_enabled << std::endl;
		fout << this->windowSettings.antialiasingLevel << std::endl;
	}

	fout.close();
}

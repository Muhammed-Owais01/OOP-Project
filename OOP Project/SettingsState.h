#pragma once
#include "State.h"
#include "DropDownList.h"

class SettingsState :
    public State
{
private:
    // Font of the text of main menu
    sf::Font font;
    sf::Text optionsText;
    sf::Clock applyClock;
    sf::Clock settingsClock;
    Button* fullScreenCheckBox;
    Button* vSyncCheckBox;
    Button* AACheckBox;

    // A map to store buttons by their name, this map stores pointers to button
    std::map<std::string, Button*> buttons;
    std::map<std::string, DropDownList*> dropDownList;
    std::vector<sf::VideoMode> modes;
    std::vector<std::string> modes_str;

    int defaultIndex;

    // initializing methods
    void initFont();
    void initText();
    void initCheckBox();
    void initModes();
public:
    // Dereference a pointer to State stack pointer as second parameter to have updated stack
    SettingsState(sf::RenderWindow* window, std::stack<State*>* states);
    ~SettingsState();

    void endState();

    sf::Vector2u stringToVector2u(const std::string& str);
    void updateKeybinds(const float& dt);
    void updateButtons(const float& dt);
    void update(const float& dt);

    void renderButtons(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};


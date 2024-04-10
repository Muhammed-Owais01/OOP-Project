#pragma once

#include "GameState.h"
#include "EditorState.h"
#include "Button.h"
#include "SettingsState.h"

class MainMenuState :
    public State
{
private:
    // background and its texture
    sf::Texture bgTexture;
    sf::RectangleShape background;

    // Font of the text of main menu
    sf::Font font;

    // A map to store buttons by their name, this map stores pointers to button
    std::map<std::string, Button*> buttons;

    // initializing methods
    void initFont();
    void initBackground();
public:
    // Dereference a pointer to State stack pointer as second parameter to have updated stack
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states);
    ~MainMenuState();

    void endState();

    void updateKeybinds(const float& dt);
    void updateButtons();
    void update(const float& dt);
    
    void renderButtons(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};


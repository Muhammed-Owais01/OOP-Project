#pragma once

#include "GameState.h"
#include "Button.h"

class MainMenuState :
    public State
{
private:
    sf::Texture bgTexture;
    sf::RectangleShape background;

    sf::Font font;

    std::map<std::string, Button*> buttons;

    void initFont();
    void initBackground();
public:
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states);
    ~MainMenuState();

    void endState();

    void updateKeybinds(const float& dt);
    void updateButtons();
    void update(const float& dt);
    
    void renderButtons(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};


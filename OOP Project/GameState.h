#pragma once

#include "State.h"
#include "Player.h"
#include "Enemy.h"
#include "PauseMenu.h"

class GameState :
    public State
{
private:
    sf::Font font;
    PauseMenu* pMenu;

    Player player;
    Enemy enemy;

    void initVariables();
    void initFont();
    void initPauseMenu();
public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    ~GameState();

    void endState();

    void updateKeybinds(const float& dt);
    void updatePause(const float& dt);
    void isPausedMenuButtonsPressed();
    void update(const float& dt);
    void render(sf::RenderTarget* target);
};


#pragma once

#include "State.h"
#include "Player.h"
#include "Enemy.h"

class GameState :
    public State
{
private:
    Player player;
    Enemy enemy;

    void initVariables();
public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states);
    ~GameState();

    void endState();

    void updateKeybinds(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target);
};


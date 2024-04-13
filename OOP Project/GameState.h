#pragma once

#include "State.h"
#include "Player.h"
#include "Enemy.h"
#include "PauseMenu.h"
#include "TileMap.h"

class GameState :
    public State
{
private:
    sf::View playerCamera;
    sf::Font font;
    PauseMenu* pMenu;

    TileMap* tileMap;

    Player player;
    Enemy enemy;

    void initVariables();
    void initView();
    void initMap();
    void initFont();
    void initPauseMenu();
public:
    GameState(StateData* stateData);
    ~GameState();

    void endState();

    void updateKeybinds(const float& dt);
    void updateView();
    void updatePause(const float& dt);
    void isPausedMenuButtonsPressed();
    void update(const float& dt);

    void render(sf::RenderTarget* target);
};


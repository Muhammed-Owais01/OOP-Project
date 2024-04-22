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
    // player camera view
    sf::View playerCamera;
    sf::Vector2i viewGridPos;

    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;

    sf::Font font;
    // Add a pause menu
    PauseMenu* pMenu;

    TileMap* tileMap;

    Player* player;

    void initDeferredRender();
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


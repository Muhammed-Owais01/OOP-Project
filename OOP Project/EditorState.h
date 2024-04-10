#pragma once
#include "State.h"
#include "Button.h"

class EditorState :
    public State
{
private:
    // Font of the text of main menu
    sf::Font font;

    // A map to store buttons by their name, this map stores pointers to button
    std::map<std::string, Button*> buttons;

    // initializing methods
    void initFont();
    void initBackground();
public:
    // Dereference a pointer to State stack pointer as second parameter to have updated stack
    EditorState(sf::RenderWindow* window, std::stack<State*>* states);
    ~EditorState();

    void endState();

    void updateKeybinds(const float& dt);
    void updateButtons();
    void update(const float& dt);

    void renderButtons(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};


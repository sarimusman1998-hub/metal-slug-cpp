#pragma once
#include "GameState.h"

class GameStateManager {
protected:                                           
    GameState* current_state;
public:

    GameStateManager() : current_state(nullptr) {
    
    }
    ~GameStateManager() {
        if (current_state)
            delete current_state;
    }
    void setState(GameState* new_state) {
        if (current_state)
            delete current_state;
        current_state = new_state;
    }
    void handleInput() {
        if (current_state) 
           current_state->handleInput();
    }
    void update() {
        if (current_state)
            current_state->update();
    }
    void render() {
        if (current_state) 
          current_state->render();
    }

    };
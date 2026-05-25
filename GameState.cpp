#include "GameState.h"
#include "Game.h"

PlayState::PlayState(Game* g) : game_ref(g) {

}
void PlayState::handleInput() { 
    game_ref->handleInput();
    }
void PlayState::update() { 
     game_ref->update();
     }
void PlayState::render() { 
     game_ref->render();  
   }

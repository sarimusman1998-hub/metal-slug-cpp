#pragma once

class Game;

class GameState {
public:
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual ~GameState() {
    
    }
};
class PlayState : public GameState {
public:
    PlayState() {
    
    }   
    void handleInput()  override {
    
    }  
    void update()       override {
    
    }  
    void render()       override {
    
    }   
};
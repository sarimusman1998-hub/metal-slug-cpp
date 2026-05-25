#pragma once
#include "Character.h"
#include "Enemy.h"


class EntityManager {
protected:
    Character** characters;    
    int  num_characters;  
    public:
    EntityManager() : characters(nullptr), num_characters(0) {
    
    }
    void initializer(Character** chars, int count) {
        characters = chars;
        num_characters = count;
    }
    Character* getCharacter(int index) {
        if (index < 0 || index >= num_characters)
             return nullptr;
        return characters[index];
    }
    bool anyAlive() const {
        for (int i = 0; i < num_characters; i++) {
            if (characters[i] && characters[i]->isAlive() && characters[i]->getLives() > 0)
                return true;
        }
        return false;
    }
};
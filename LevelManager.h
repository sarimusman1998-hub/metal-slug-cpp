#pragma once
#include "Level.h"

class LevelManager {
protected:                                            
    Level* current_level;
    int    current_level_num;
public:
    LevelManager() : current_level(nullptr), current_level_num(0) {
    
    }
    ~LevelManager() {
        if (current_level)
            delete current_level;
    }
    void setLevel(Level* level, int level_num) {
        if (current_level)
            delete current_level;
        current_level = level;
        current_level_num = level_num;
    }
    Level* getLevel() const { 
       return current_level;
              }
    int getLevelNum() const { 
      return current_level_num;      
      }
};
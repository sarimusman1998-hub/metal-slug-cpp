#pragma once
#include "Enemy.h"

class GrudgeRegistry {
private:
    static const int MAX_GRUDGE_ENEMIES = 15;
    Enemy* pool[MAX_GRUDGE_ENEMIES];
    float  timers[MAX_GRUDGE_ENEMIES];
    int    count;
public:
    GrudgeRegistry();
    ~GrudgeRegistry();
    int  getCount() const;
    void registerEnemy(Enemy* e);
    void update(float dt, Character* player,
        Enemy** level_enemies, int& enemy_count, int max_enemies);
    void clear();
};
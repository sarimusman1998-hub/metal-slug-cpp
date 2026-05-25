#pragma once
#include "Enemy.h"

class CowardiceReserve {
private:
    static const int MAX_RESERVE = 67;
    static const int TRIGGER_COUNT = 10;
    Enemy* reserve[MAX_RESERVE];
    int    count;
    bool   counterattack_active;
    float  announcement_timer;
    void releaseAll(Character* player,  Enemy** level_enemies, int& enemy_count, int max_enemies);
public:
    CowardiceReserve();
    ~CowardiceReserve();
    int  getCount()    const;
    bool isTriggered() const;
    void addEnemy(Enemy* e);
    void update(float dt, Character* player,  Enemy** level_enemies, int& enemy_count, int max_enemies);
    void clear();
};
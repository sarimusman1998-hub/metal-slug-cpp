#include "CowardiceReserve.h"

CowardiceReserve::CowardiceReserve(): count(0), counterattack_active(false), announcement_timer(0.0f)
{
    for (int i = 0; i < MAX_RESERVE; i++) 
    {
        reserve[i] = nullptr;
    }

}
CowardiceReserve::~CowardiceReserve() 
{
    clear();
}
int  CowardiceReserve::getCount()    const 
{ 
    return count; 
}
bool CowardiceReserve::isTriggered() const 
{ 
    return counterattack_active;

}


void CowardiceReserve::addEnemy(Enemy* E) {
    if (!E || count >= MAX_RESERVE) { delete E; return; }
      E->setPowerBoost(2.0f);
      E->forceRevive(E->getHP()*1.67);
    reserve[count++] = E;
    if (count >= TRIGGER_COUNT) counterattack_active = true;
}


void CowardiceReserve::update(float dt, Character* player,
    Enemy** level_enemies, int& enemy_count, int max_enemies)
{
    if (!counterattack_active) return;
    {
        announcement_timer += dt;
    }

    if (announcement_timer < 1.5f) return;

    releaseAll(player, level_enemies, enemy_count, max_enemies);
}



void CowardiceReserve::releaseAll(Character* player, Enemy** level_enemies, int& enemy_count, int max_enemies)
{
    float base_x = player->getPosX() - 380.0f;
    if (base_x < 50.0f) base_x = 50.0f;

    for (int i = 0; i < count; i++)
    {
        if (!reserve[i]) continue;

     
        float sx = base_x + (float)i * 12.0f;

        reserve[i]->setPosition(sx, player->getPosY());

        bool added = false;
        for (int j = 0; j < max_enemies; j++)
        {
            if (level_enemies[j] == nullptr || !level_enemies[j]->isAlive())
            {
                if (level_enemies[j]) delete level_enemies[j];
                level_enemies[j] = reserve[i];
                if (j >= enemy_count) enemy_count = j + 1;
                reserve[i] = nullptr;
                added = true;
                break;
            }
        }

        if (!added) { delete reserve[i]; reserve[i] = nullptr; }
    }

    count = 0;
    counterattack_active = false;
    announcement_timer = 0.0f;
}



void CowardiceReserve::clear() {
    for (int i = 0; i < MAX_RESERVE; i++) {

        if (reserve[i])
        {
            delete reserve[i];
            reserve[i] = nullptr;
        }
    }
    count = 0; 
    counterattack_active = false;
    announcement_timer = 0.0f;
}
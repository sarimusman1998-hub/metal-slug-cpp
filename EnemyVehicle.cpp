#include "EnemyVehicle.h"
#include "Level.h"


void FlyingTara::move(Character* player, Level* level, float dt)
{
 
    float effective_speed = speed * power_boost;

   
    if (hover_y < 80.0f) hover_y = 80.0f;
    float maxH = (float)(level->getHeight() - 5) * (float)level->getCellSize();
    if (hover_y > maxH) hover_y = maxH;

   
    float rise = 120.0f * dt;
    if (pos_y > hover_y + 2.0f) pos_y -= rise;
    else if (pos_y < hover_y - 2.0f) pos_y += rise;

   
    patrol_timer += dt;
    if (patrol_timer >= 3.5f) {
        patrol_timer = 0.0f;
        patrol_dir = (patrol_dir > 0.0f) ? -1.0f : 1.0f;
    }

    velocity_x = patrol_dir * effective_speed;
    velocity_y = 0.0f;
}
void M15ABradley::move(Character* player, Level* level, float dt) {
    float dx = player->getPosX() - pos_x;
    if (dx > 350.0f) 
      velocity_x = speed * power_boost;
    else if (dx < -350.0f) 
      velocity_x = -speed * power_boost;
    else                  
       velocity_x = 0.0f;
    checkAndJump(level);
}
void EnemySub::move(Character* player, Level* level, float dt) {
    
    float old_vx = velocity_x;
    float dx = player->getPosX() - pos_x;
    if (dx > 150.0f)
        velocity_x = speed * power_boost;
    else if (dx < -150.0f)
        velocity_x = -speed * power_boost;
    else
        velocity_x = 0.0f;
    if (velocity_x > 0.0f)
        last_dir = 1;
    else if (velocity_x < 0.0f)
        last_dir = -1;

    if (turn_timer <= 0.0f) {
        if ((old_vx > 0.0f && velocity_x < 0.0f) || (old_vx < 0.0f && velocity_x > 0.0f)) {
            turn_timer = 1.0f;  
        }
    }
    if (turn_timer > 0.0f) {
        current_anim = 1;        // TURN 
        turn_timer -= dt;
        if (turn_timer <= 0.0f)
            turn_timer = 0.0f;
    }

    surface_timer += dt;
    if (!surfacing && surface_timer >= 6.0f) { 
       surfacing = true;
        surface_timer = 0.0f;
        surface_target_y = pos_y - 80.0f;
       }
    if (surfacing) {
        if (pos_y > surface_target_y)
            pos_y -= 2.5f;
        else
            surfacing = false;
    }
    if (pos_y < min_y) {
        pos_y = min_y;
        surfacing = false;
    }
    velocity_y = 0.0f;

}
void addToPool(Projectile* p, Projectile** pool, int& proj_count, int max_proj) {
    for (int i = 0; i < max_proj; i++) {
        if (pool[i] == nullptr || !pool[i]->isActive()) {
            delete pool[i];
            pool[i] = p;
            return;
        }
    }
    delete p; 
}

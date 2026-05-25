#include "Character.h"
#include "Level.h"
#include "Enemy.h"                          
#include "Scoring_Bonuses.h" 

void Character::applyGravity(Level* level){

    const float gravity = 0.5f;
    const float terminal_vel = 10.0f;
    int cs = level->getCellSize();    // take frm lvl


    //Water
    int center_row = (int)(pos_y+character_height * 0.6f)/cs;
    int center_col = (int)(pos_x + character_width * 0.5f)/cs;
    char center_block = level->getBlock(center_row,center_col);

    if (center_block == 'w') { //in water
        in_water = true;
        on_ground = false;
        jump_count = 0;

        velocity_y -= 0.3f;
        velocity_y *= 0.82f;

        if (velocity_y < -4.0f) 
        
           velocity_y = -4.0f;

        if (velocity_y > 4.0f)
        
            velocity_y = 4.0f;


        pos_y += velocity_y;
        applyHorizontalCollision(level);
        return;
    }
    in_water = false;
    // Vert

    velocity_y += gravity;

    velocity_y > terminal_vel ? velocity_y = terminal_vel : velocity_y;  //no bs infinite speed and acc



    float next_y = pos_y + velocity_y;

    // Check block below feet
    int foot_row = (int)(next_y + character_height) / cs;
    int col_left = (int)(pos_x) / cs;
    int col_mid = (int)(pos_x + character_width / 2) / cs;
    int col_right = (int)(pos_x + character_width) / cs;



    //bounds
    if (foot_row < 0 || foot_row >= level->getHeight()) {  // no escape from za wardo
        on_ground = true;
        velocity_y = 0;
        jump_count = 0;
        return;
    }

    col_left < 0 ? col_left = 0 : col_left;
    col_right >= level->getWidth() ? col_right = level->getWidth() - 1 : col_right;   // no teleportaion or loop on the boundaries
    col_mid < 0 ? col_mid = 0 : col_mid;
    col_mid >= level->getWidth() ? col_mid = level->getWidth() - 1 : col_mid;             // 3 point mth idk i remember from THE TUMBLE 


    char block_left = level->getBlock(foot_row, col_left);
    char block_mid = level->getBlock(foot_row, col_mid);
    char block_right = level->getBlock(foot_row, col_right);


    bool blocked_below = ((block_left == 'g' || block_left == 's' || block_left == 'b') || (block_mid == 'g' || block_mid == 's' || block_mid == 'b') || (block_right == 'g' || block_right == 's' || block_right == 'b'));


    if (blocked_below && velocity_y > 0)     // if falling and neche block aa jata he
    {
        on_ground = true;
        velocity_y = 0;
        pos_y = (foot_row * cs) - character_height;  // snap above block
    }
    else
    {
        pos_y = next_y;
        !blocked_below ? on_ground = false : on_ground;   
    }

}

void Character::applyHorizontalCollision(Level* level)
{

    int cs = level->getCellSize();

    float next_x = pos_x + velocity_x;


    next_x < 0 ? next_x = 0 : next_x;

    float max_x = (float)(level->getWidth() * cs - character_width);         //why are u running
    next_x > max_x ? next_x = max_x : next_x;


    int top_row = (int)(pos_y + 5) / cs;  // same shit diffrent function
    int mid_row = (int)(pos_y + character_height / 2) / cs;
    int bot_row = (int)(pos_y + character_height - 5) / cs;


    if (velocity_x > 0)  // RIGHT
    {
        int right_col = (int)(next_x + character_width) / cs;
        if (right_col < level->getWidth())
        {
            char top = level->getBlock(top_row, right_col);
            char mid = level->getBlock(mid_row, right_col);
            char bottom = level->getBlock(bot_row, right_col);
            if ((top == 'g' || top == 's' || top == 'b') || (mid == 'g' || mid == 's' || mid == 'b') || (bottom == 'g' || bottom == 's' || bottom == 'b'))
            {
                next_x = (right_col * cs) - character_width - 1;
            }
        }
    }
    else if (velocity_x < 0)  // LEFT
    {
        int left_col = (int)(next_x) / cs;
        if (left_col < level->getWidth())
        {
            char top = level->getBlock(top_row, left_col);
            char mid = level->getBlock(mid_row, left_col);
            char bottom = level->getBlock(bot_row, left_col);
            if ((top == 'g' || top == 's' || top == 'b') || (mid == 'g' || mid == 's' || mid == 'b') || (bottom == 'g' || bottom == 's' || bottom == 'b'))
            {
                next_x = (left_col + 1) * cs + 1;
            }
        }
    }

    pos_x = next_x;     // finally after all dat

}
void  Character::updateTimers(float dt)
{
    // Damage state timer
    if (damage_timer > 0)
    {
        damage_timer -= dt;
        if (damage_timer <= 0)//clamp
        {
            // Timer expired: reset to normal if not hit again
            damage_state = 0;
            damage_timer = 0;
        }
    }

    // Character state timer
    if (character_state != 0)
    {
        state_timer -= dt;  //clamp
        if (state_timer <= 0)
        {
            character_state = 0;
            state_timer = 0;
        }
    }

    // Special  timer
    if (special_active)
    {
        special_timer -= dt;
        if (special_timer <= 0)//clamp
        {
            special_active = false;
            special_timer = 0;

            Fio* fio = dynamic_cast<Fio*>(this);
            if (fio && fio->getSuperchargective()) {
                fio->deactivateSupercharge();
            }

            Marco* marco = dynamic_cast<Marco*>(this);
            if (marco)
               marco->setDualfireactive(false);

            Eri* eri = dynamic_cast<Eri*>(this);
            if (eri)
               eri->setDoublegrenadeactive(false);


        }
    }

    // its vincing time
    if (is_invincible)
    {
        invincible_timer -= dt;
        if (invincible_timer <= 0)//clamp
        {
            is_invincible = false;
            invincible_timer = 0;
        }
    }

    // Melee cooldown

    if (melee_timer > 0)
    {
        melee_timer -= dt;
        if (melee_timer <= 0)//clamp
        {
            melee_timer = 0;
        }
    }

    if (grenade_cooldown_timer > 0) {
        grenade_cooldown_timer -= dt;
        if (grenade_cooldown_timer <= 0)
            grenade_cooldown_timer = 0;
    }

    // Weapon fire timer
    if (current_weapon)
        current_weapon->updateTimer(dt);

    // fat (fit) check
    if (saturation >= 10 && !is_fat)
        is_fat = true;

    // Anime timer
    animation_timer += dt;
    if (velocity_x == 0 && velocity_y == 0)
        playIdleAnim(dt);  // CHANGED


    //Double Jump Timer
    if (jump_buffer > 0) {
        jump_buffer -= dt;
        if (jump_buffer < 0) {
            jump_buffer = 0;
          }
    }

    if (on_ground)
        jump_count = 0;
}
void  Character::spawnGrenade(Projectile** pool, int& count, int max_proj, float start_x, float start_y, float vx, float vy, sf::Texture& grenade_tex) {
    Projectile* g = new Projectile(start_x, start_y, vx, vy, 20.0f, 16, 16);

   g->setExplosive(true, grenade_blast_radius);
   g->setGrenade(true);
   Eri* eri_check = dynamic_cast<Eri*>(this);
   if (eri_check)
     g->setisFire(true);

    g->sprite.setTexture(grenade_tex);   // txture


    for (int i = 0; i < max_proj; i++)
    {
        if (pool[i] == 0 || !pool[i]->isActive())
        {
            if (pool[i] != 0) delete pool[i];
            pool[i] = g;
            return;
        }
    }
    delete g;
}
void Character::throwGrenade(Projectile** pool, int& count, int max_proj, sf::Texture& grenade_tex)
{
    if (!is_alive) return;

    if (grenade_count <= 0) return;

    if (grenade_cooldown_timer > 0) return;


    grenade_count--;
    old_grenade_count = grenade_count;
    grenade_cooldown_timer = 0.5f;

    // ballistic arc     aayy  parabola   


    // Initial velocity: forward + upward
    float vx = (facing_direction == 1) ? 5.0f : -5.0f;
    float vy = -8.0f;



    float start_x = pos_x + (facing_direction == 1 ? character_width + 10 : -10);
    float start_y = pos_y + (character_height / 2);

    spawnGrenade(pool, count, max_proj, start_x, start_y, vx, vy, grenade_tex);
    Eri* eri = dynamic_cast<Eri*>(this);
    if (eri && eri->getDoublegrenadeActive()) {
        float extra_vx = (facing_direction == 1) ? 1.6f : -1.6f;
        spawnGrenade(pool, count, max_proj, start_x, start_y, vx + extra_vx, vy, grenade_tex);

    }



}//yeetus
bool Character::melee(Enemy** enemies, int max_enemies, Scoring* game_score, bool player_airborne) {
    if (!is_alive)
        return false;
    if (character_state == 2) {
        return false;
    }
    if (melee_timer > 0)
        return false;
    melee_timer = melee_cooldown;

    const float MELEE_RANGE = 185.0f;
    const float MELEE_RANGE_Y = character_height * 1.6f;
    bool hit_any = false;

    for (int i = 0;i < max_enemies;i++) {
        if (enemies[i] == 0 || !enemies[i]->isAlive())
            continue;

        float dx = (enemies[i]->getPosX() + enemies[i]->getEnemyWidth() * 0.5f) - (pos_x + character_width * 0.5f);
        float dy = (enemies[i]->getPosY() + enemies[i]->getEnemyHeight() * 0.5f) - (pos_y + character_height * 0.5f);
        bool in_front = (facing_direction == 1) ? (dx > -40.0f) : (dx < 40.0f);

        if (!in_front)
            continue;

        float abs_dx = (dx > 0) ? dx : -dx;
        float abs_dy = (dy > 0) ? dy : -dy;

        if (abs_dx <= MELEE_RANGE && abs_dy <= MELEE_RANGE_Y) {
            if (enemies[i]->blocksBullet(1.0f, pos_x) && !canPierceShield())
            continue;
                enemies[i]->takeDamage(2.0f);
            hit_any = true;
            if (!enemies[i]->isAlive() && game_score) {
                game_score->addScore(enemies[i]->getScorevalue(), "Kill");
                game_score->addScore(50, "Melee");
                if (player_airborne)
                    game_score->addScore(100, "Aerial");
            }
        }
    }
    return hit_any;
}
void FusionCompanion::activate(float px, float py, Character** chars, int num_chars) {
    if (is_active_companion)
        return;

    pos_x = px - 80.0f;
    pos_y = py;
    velocity_x = velocity_y = 0.0f;
    is_alive = true;
    is_active_companion = true;
    active_timer = duration; //30 seconds
    ai_fire_timer = ai_melee_timer = ai_grenade_timer = 0.0f;

    //Averages
    float sum_speed = 0.0f;
    float sum_cooldown = 0.0f;
    int count = 0;
    for (int i = 0; i < num_chars; i++) {
        if (!chars[i] || !chars[i]->isAlive()) 
        continue;

            sum_speed += chars[i]->getSpeed();
            sum_cooldown += (chars[i]->getCurrentWeapon() ? chars[i]->getCurrentWeapon()->getFireCooldown() : 0.25f);
            count++;
    }
    if (count > 0) {
        speed = sum_speed / count;
        if (current_weapon)
            current_weapon->setFireCooldown(sum_cooldown / count);
    }
}
void FusionCompanion::updateAI(Character* player, Level* level, Enemy** enemies, int max_enemies, Projectile** pool, int& proj_count, int max_proj, Scoring* score, float dt) {
    if (!is_active_companion || !is_alive)
        return;
    active_timer -= dt;
    if (active_timer <= 0.0f) {
        is_active_companion = false;
        is_alive = false;
        return;
    }

    applyGravity(level);

    float dx = player->getPosX() - pos_x;
    if (dx > 120.0f)
        moveRight(dt);
    else if (dx < -120.0f)
        moveLeft(dt);
    else
        stopHorizontal();


    //Check and jump simple
    if (on_ground && velocity_x != 0.0f) {
        int cs = level->getCellSize();
        int col = (int)(pos_x + (velocity_x > 0 ? character_width + 4 : -4)) / cs;
        int row = (int)(pos_y + character_height * 0.5f) / cs;
        if (col >= 0 && col < level->getWidth()) {
            char blk = level->getBlock(row, col);
            if (blk == 'g' || blk == 's' || blk == 'b')
                jump(dt);
        }
    }

    applyHorizontalCollision(level);


    Enemy* target = nullptr;
    float ideal_dist = 600.0f;
    for (int e = 0; e < max_enemies;e++) {
        Enemy* e_1 = enemies[e];
        if (!e_1 || !e_1->isAlive())
            continue;
        float e_1_x = e_1->getPosX() - pos_x;
        float e_1_y = e_1->getPosY() - pos_y;
        float dist = sqrtf(e_1_x * e_1_x + e_1_y * e_1_y);
        if (dist < ideal_dist) {
            ideal_dist = dist;
            target = e_1;
        }


    }
    if (!target)
        return;

    facing_direction = (target->getPosX() > pos_x) ? 1 : 0;

    updateTimers(dt);
    shoot(pool, proj_count, max_proj);

    ai_melee_timer += dt;
    if (ai_melee_timer >= melee_cooldown) {
        ai_melee_timer = 0.0f;
        melee(enemies, max_enemies, score, !on_ground);
    }
    ai_grenade_timer += dt;

    if (ai_grenade_timer >= 4.0f && ideal_dist > 150.0f && grenade_count > 0) {
        ai_grenade_timer = 0.0f;
        throwGrenade(pool, proj_count, max_proj, grenade_tex);
    }

}



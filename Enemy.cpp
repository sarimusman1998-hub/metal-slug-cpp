#include "Enemy.h"
#include "Level.h"


void Enemy::applyGravity(Level* level)
{
    const float gravity = 0.5f;
    const float terminal_vel = 10.0f;
    int cs = level->getCellSize();

    //Water
    int center_row = (int)(pos_y + enemy_height * 0.6f) / cs;
    int center_col = (int)(pos_x + enemy_width * 0.5f) / cs;
    char center_block = level->getBlock(center_row, center_col);

    if (center_block == 'w') {

        on_ground = false;
        velocity_y -= 0.25f;
        velocity_y *= 0.85f;
        if (velocity_y < -3.0f)
           velocity_y = -3.0f;
        if (velocity_y > 3.0f)
           velocity_y = 3.0f;
        pos_y += velocity_y;
        return;
    }
    in_water = false;

    velocity_y += gravity;
    if (velocity_y > terminal_vel) velocity_y = terminal_vel;

    float next_y = pos_y + velocity_y;

    int foot_row = (int)(next_y + enemy_height) / cs;
    int col_left = (int)(pos_x) / cs;
    int col_mid = (int)(pos_x + enemy_width / 2) / cs;
    int col_right = (int)(pos_x + enemy_width) / cs;

    // Clamp columns
    if (col_left < 0) col_left = 0;
    if (col_right >= level->getWidth()) col_right = level->getWidth() - 1;
    if (col_mid < 0) col_mid = 0;
    if (col_mid >= level->getWidth()) col_mid = level->getWidth() - 1;

    if (foot_row < 0 || foot_row >= level->getHeight())
    {
        on_ground = true;
        velocity_y = 0;
        return;
    }

    char bl = level->getBlock(foot_row, col_left);
    char bm = level->getBlock(foot_row, col_mid);
    char br = level->getBlock(foot_row, col_right);

    bool blocked = (bl == 'g' || bl == 's' || bl == 'b') ||
        (bm == 'g' || bm == 's' || bm == 'b') ||
        (br == 'g' || br == 's' || br == 'b');

    if (blocked && velocity_y > 0)
    {
        on_ground = true;
        velocity_y = 0;
        pos_y = (float)(foot_row * cs) - enemy_height;
    }
    else
    {
        pos_y = next_y;
        on_ground = false;
    }
}
void Enemy::checkAndJump(Level*level) {
    if (velocity_y < -5.0f) 
       return;
    int c_size = level->getCellSize();

    int check_col;
    if (velocity_x > 0) {
    check_col = (pos_x+enemy_width+4)/c_size;
     }
    else if (velocity_x < 0) {
    check_col = (pos_x - 4)/c_size;
    }
    else {
        return;
    }
    int mid_height = (int)(pos_y + enemy_height * 0.5f)/c_size;
    int bot_height = (int)(pos_y + enemy_height - 4.0f) / c_size;

    if (check_col < 0 || check_col >= level->getWidth())
       return;

       char block = level->getBlock(mid_height,check_col);
       char block_mid = level->getBlock(bot_height, check_col);

       if (block == 'g' || block == 's' || block == 'b' || block_mid == 'g' || block_mid == 's' || block_mid == 'b') {
           if (in_water) {
               velocity_y = -10.0f;
          }
          else{
          velocity_y = -15.0f;  
            }
           on_ground = false;
       }
}
void Enemy::applyHorizontalCollision(Level* level)
{
    int cs = level->getCellSize();

    float next_x = pos_x + velocity_x;

    if (next_x < 0.0f) next_x = 0.0f;
    
    float max_x = (float)(level->getWidth() * cs) - enemy_width;
    if (next_x > max_x) next_x = max_x;

    int top_row = (int)(pos_y + 4.0f) / cs;
    int mid_row = (int)(pos_y + enemy_height * 0.5f) / cs;
    int bot_row = (int)(pos_y + enemy_height - 4.0f) / cs;

    if (top_row < 0) top_row = 0;
    if (bot_row >= level->getHeight()) bot_row = level->getHeight() - 1;
    if (mid_row < 0) mid_row = 0;
    if (mid_row >= level->getHeight()) mid_row = level->getHeight() - 1;

    if (velocity_x > 0.0f) {
     
        int right_col = (int)(next_x + enemy_width) / cs;
        if (right_col < level->getWidth()) {
            char t = level->getBlock(top_row, right_col);
            char m = level->getBlock(mid_row, right_col);
            char b = level->getBlock(bot_row, right_col);
            if ((t == 'g' || t == 's' || t == 'b') ||
                (m == 'g' || m == 's' || m == 'b') ||
                (b == 'g' || b == 's' || b == 'b'))
            {
                next_x = (float)(right_col * cs) - enemy_width - 1.0f;
                velocity_x = 0.0f;
            }
        }
    }
    else if (velocity_x < 0.0f) {
        int left_col = (int)(next_x) / cs;
        if (left_col >= 0 && left_col < level->getWidth()) {
            char t = level->getBlock(top_row, left_col);
            char m = level->getBlock(mid_row, left_col);
            char b = level->getBlock(bot_row, left_col);
            if ((t == 'g' || t == 's' || t == 'b') ||
                (m == 'g' || m == 's' || m == 'b') ||
                (b == 'g' || b == 's' || b == 'b'))
            {
                next_x = (float)((left_col + 1) * cs) + 1.0f;
                velocity_x = 0.0f;

                if (b == 'g' || b == 's' || b == 'b') {
                    velocity_y = in_water ? -10.0f : -15.0f;
                    on_ground = false;
                }
            }
        }
    }

    pos_x = next_x;
}
void DefaultAIState::execute(Enemy* self, Character* player, Level* level, Projectile** pool, int& proj_count, int  max_proj, sf::Texture& bullet_tex, float dt) {
    self->move(player, level, dt);
    self->attack(player, pool, proj_count, max_proj, bullet_tex,  dt);
}
void Martian::move(Character* player, Level* level, float dt)
{


    float effective_speed = speed * power_boost;

    if (phase == 1)
    {
        playAnim(FLY);   // FLY U NIGR

        int cs = level->getCellSize();


        int col = (int)(pos_x + enemy_width * 0.5f) / cs;
        if (col < 0)                  col = 0;
        if (col >= level->getWidth()) col = level->getWidth() - 1;

        int terrain_row = level->getHeight() - 1;
        for (int r = 0; r < level->getHeight(); r++)
        {
            char b = level->getBlock(r, col);
            if (b == 'g' || b == 's' || b == 'b')
            {
                terrain_row = r;
                break;
            }
        }
        float target_y = (float)((terrain_row - 6) * cs);
        float min_y = (float)(1 * cs);              // never above row 1
        if (target_y < min_y) target_y = min_y;


        if (!fly_height_initialized)
        {
            fly_height = target_y;
            fly_height_initialized = true;
        }
        else {
            fly_height += (target_y - fly_height) * 0.08f;
        }
        patrol_timer += dt;
        if (patrol_timer >= 2.0f)
        {
            patrol_timer = 0.0f;
            patrol_dir = -patrol_dir;
        }
        float dx = player->getPosX() - pos_x;
        float drift = (dx > 0 ? 0.3f : -0.3f) * effective_speed;
        velocity_x = patrol_dir * effective_speed * 0.6f + drift;


        pos_y = fly_height;
        velocity_y = 0;

        if (hp <= 1)
        {
            phase = 2;
            pod_active = false;
            fly_height = pos_y;    // SPLAT (THAT MARTIAN FROM THAT SHOW WITH STAN YKYK  FAMILY GUY LOOKIN)
        }

    }
    else // phase 2
    {
        patrol_timer += 1.0f / 60.0f;
        playAnim(WALK);   // WALK U NIGR
        if (patrol_timer >= 2.0f) {
            patrol_timer = 0.0f;
            patrol_dir = -patrol_dir;
        }
        float dx = player->getPosX() - pos_x;
        float abs_dx = (dx >= 0.0f) ? dx : -dx;

        if (abs_dx > 200.0f) {
            if (dx > 0)
                velocity_x = effective_speed * 0.7;
            else
                velocity_x = -effective_speed * 0.7;
        }
        else {
            velocity_x = patrol_dir * effective_speed * 0.5f;
        }
        checkAndJump(level);
    }


}
#include "ParaTrooper.h"
#include "Level.h"

void Paratrooper::move(Character* player, Level* level, float dt) {
    if (is_descending) {
        velocity_y = descent_speed;
        velocity_x = 0.0f;
        pos_y += velocity_y;
        int cs = level->getCellSize();

        int foot_row = (int)(pos_y + enemy_height) / cs;
        int col_mid = (int)(pos_x + enemy_width * 0.5f) / cs;

        if (foot_row >= 0 && foot_row < level->getHeight() &&
            col_mid >= 0 && col_mid < level->getWidth()) {

            char block = level->getBlock(foot_row, col_mid);

            if ((block == 'g' || block == 's' || block == 'b') && velocity_y > 0) {
                is_descending = false;
                on_ground = true;
                velocity_y = 0;
                pos_y = (float)(foot_row * cs) - enemy_height;
            }
        }

        return;
    }

    float effective_speed = speed * power_boost;

    float dx = player->getPosX() - pos_x;
    float abs_dx = dx < 0 ? -dx : dx;
    if (abs_dx > 55.0f)
        velocity_x = (dx < 0) ? -effective_speed : effective_speed;
    else
        velocity_x = 0.0f;

    checkAndJump(level);

}
#include "Projectile.h"
#include "Level.h" 
void Projectile::update(Level* level)
{
    if (!is_active) return;

    if (lifespan > 0.0f) {

        lifespan_timer += 1.0f / 60.0f;
        if (lifespan_timer >= lifespan) {
            is_active = false;
            return;
        }
    }
    pos_x += velocity_x;
    pos_y += velocity_y;

    if (pos_x < -200 || pos_x > level->getWidth() * level->getCellSize() + 200)
    {
        is_active = false;
        return;
    }
    if (pos_y < -200 || pos_y > level->getHeight() * level->getCellSize() + 200)
    {
        is_active = false;
        return;
    }

    int row = (int)(pos_y + proj_height / 2) / level->getCellSize();
    int col = (int)(pos_x + proj_width / 2) / level->getCellSize();

    char block = level->getBlock(row, col);

    if (block == 'g' || block == 's' || block == 'b')
    {
        is_active = false;

        if (is_explosive)
        {
            explode_x = pos_x + proj_width / 2.0f;
            explode_y = pos_y + proj_height / 2.0f;
            just_exploded = true;
            int radius_blocks = 1;

            for (int r = row - radius_blocks; r <= row + radius_blocks; r++)
            {
                for (int c = col - radius_blocks; c <= col + radius_blocks; c++)
                {
                    int dr = r - row;
                    int dc = c - col;

                    if (dr * dr + dc * dc <= radius_blocks * radius_blocks)
                        level->destroyBlock(r, c);
                }

                }
            }
        }
    }
#include "EnemyAIState.h"
#include "Enemy.h"

void execute(Enemy* self, Character* player, Level* level, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex){
    self->move(player, level);
    self->attack(player, pool, proj_count, max_proj, bullet_tex);
}
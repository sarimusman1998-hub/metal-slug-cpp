#pragma once
#include <SFML/Graphics.hpp>


class Enemy;
class Character;
class Level;
class Projectile;

class EnemyAIState {
public:
    virtual void execute(  Enemy* self, Character* player,Level* level, Projectile** pool,int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) = 0;
    virtual ~EnemyAIState() {
    
    }
};
class DefaultAIState : public EnemyAIState {
public:
    void execute( Enemy* self,Character* player, Level* level, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override;

};


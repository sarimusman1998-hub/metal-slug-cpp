#pragma once
#include "Character.h"
#include "Projectile.h"
#include "Enemy.h"

class Level;

class Paratrooper : public Enemy {
protected:

    bool is_descending;
    float descent_speed;

    int infantry_type;      // 0=rebel,1=shielded,2=bazooka,3=grenade

    float attack_timer;
    bool  shield_active;

public:
    Paratrooper(float x, float y)  // where it faal frm
    {
        pos_x = x;
        pos_y = y;
        speed = 1.5f;
        is_descending = true;
        descent_speed = 2.0f;

        infantry_type = rand() % 4;     // random infantry type 
        activation_range = 1800.0f;
        attack_timer = 0;

        // Score = base infantry score + 25
        int base_scores[] = { 50, 75, 100, 100 };

        score_value = base_scores[infantry_type] + 25;

        hp = (infantry_type == 1) ? 5.0f : 2.0f;  // shielded gets 5
        shield_active = (infantry_type == 1);


        texture.loadFromFile("25I-0555_25I-1034_Assets/paratrooper.png");
        sprite.setTexture(texture);
    }
    void move(Character* player, Level* level, float dt) override;
    bool usesGravity() const override {
        return !is_descending;
    }
    int getEnemyTypeID() const {
        return 4;
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override {
        if (is_descending) return;
        attack_timer += 1.0f / 60.0f;
        if (attack_timer < 1.0f) return;
        attack_timer = 0;
        float dx = player->getPosX() - pos_x;           
        float dy = player->getPosY() - pos_y;           
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 1) dist = 1;
        if (dist > 500) return;
        Projectile* p = new Projectile(pos_x + 20, pos_y + 20, (dx / dist) * 5.0f, (dy / dist) * 5.0f, 3.0f, 12, 8);
        p->setFromEnemy(true); p->setSpriteTexture(bullet_tex); 
        for (int i = 0; i < max_proj; i++) {
            if (pool[i] == 0 || !pool[i]->isActive()) {
                if (pool[i]) delete pool[i]; pool[i] = p; return;
            }
        }
        delete p;
    }
}; //attack end
class ShieldedParatrooper : public Paratrooper {
public:
    ShieldedParatrooper(float x, float y) : Paratrooper(x, y) {
        infantry_type = 1;
        score_value = 100; hp = 5.0f; shield_active = true;
    }
    bool blocksBullet(float proj_vx, float shooter_x) const override {  
        if (!shield_active) return false;
        return (proj_vx > 0 && shooter_x < pos_x) || (proj_vx < 0 && shooter_x > pos_x);
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override {
        if (is_descending) return;
        attack_timer += 1.0f / 60.0f;
        if (attack_timer < 1.5f) return;
        attack_timer = 0;
        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 1) dist = 1;
        if (dist > 500) return;
        Projectile* p = new Projectile(pos_x + 20, pos_y + 20, (dx / dist) * 4.0f, (dy / dist) * 4.0f, 3.0f, 12, 8);
        p->setFromEnemy(true); p->setSpriteTexture(bullet_tex);
        for (int i = 0; i < max_proj; i++) {
            if (pool[i] == 0 || !pool[i]->isActive()) {
                if (pool[i]) delete pool[i]; pool[i] = p; return;
            }
        }
        delete p;
    }
    int getEnemyTypeID() const {
        return 4;
    }
};
class BazookaParatrooper : public Paratrooper {
public:
    BazookaParatrooper(float x, float y) : Paratrooper(x, y) {
        infantry_type = 2;
        score_value = 125; hp = 2.0f; shield_active = false;
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override {
        if (is_descending) return;
        attack_timer += 1.0f / 60.0f;
        if (attack_timer < 2.5f) return;
        attack_timer = 0;
        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 1) dist = 1;
        if (dist > 500) return;
        float vx = (dx / dist) * 5.0f;
        Projectile* p = new Projectile(pos_x + 20, pos_y, vx, -8.0f, 5.0f, 20, 10);
        p->setFromEnemy(true); p->setExplosive(true, 3.0f); p->setGrenade(true);
        p->setSpriteTexture(rocket_tex);
        for (int i = 0; i < max_proj; i++) {
            if (pool[i] == 0 || !pool[i]->isActive()) {
                if (pool[i]) delete pool[i]; pool[i] = p; return;
            }
        }
        delete p;
    }
    int getEnemyTypeID() const {
        return 4;
    }
};
class GrenadeParatrooper : public Paratrooper {
public:
    GrenadeParatrooper(float x, float y) : Paratrooper(x, y) {
        infantry_type = 3;
        score_value = 125; hp = 2.0f; shield_active = false;
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override {
        if (is_descending) return;
        attack_timer += 1.0f / 60.0f;
        if (attack_timer < 2.0f) return;
        attack_timer = 0;
        float dx = player->getPosX() - pos_x;
        if (sqrt(dx * dx + (player->getPosY() - pos_y) * (player->getPosY() - pos_y)) > 500) return;
        float vx_g = (dx > 0) ? 4.0f : -4.0f;
        Projectile* p = new Projectile(pos_x + 20, pos_y, vx_g, -7.0f, 20.0f, 16, 16);
        p->setFromEnemy(true); p->setExplosive(true, 3.0f); p->setGrenade(true);
        p->setSpriteTexture(grenade_tex);
        for (int i = 0; i < max_proj; i++) {
            if (pool[i] == 0 || !pool[i]->isActive()) {
                if (pool[i]) delete pool[i]; pool[i] = p; return;
            }
        }
        delete p;
    }
    int getEnemyTypeID() const {
        return 4;
    }
};
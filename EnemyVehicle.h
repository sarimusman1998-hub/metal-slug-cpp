#pragma once
#include "Enemy.h"
#include <cmath>

using namespace sf;
void addToPool(Projectile* p, Projectile** pool, int& proj_count, int max_proj);

class FlyingTara : public Enemy {
float hover_y, attack_timer,patrol_dir, drift_timer;
float patrol_timer;
Texture g_tex;
public:
    FlyingTara(float x, float y) {
       pos_x = x;
       pos_y = y;
       hover_y = y - 80.0f;
       patrol_dir = 1.0f;
       attack_timer = 0.0f;
       drift_timer = 0.0f;
       hp = 7.0f;
       speed = 2.2f;
       score_value = 150;

       patrol_timer = 0.0f;
       texture.loadFromFile("Assets/flying_tara.png");
       sprite.setTexture(texture);
       //sprite.setScale((float)enemy_width / (float)texture.getSize().x,(float)enemy_height / (float)texture.getSize().y);
       g_tex.loadFromFile("Assets/grenade_tex.png");
       frame_width = 80;   
       frame_height = 40;   
       addClip(0, 8, 8.0f); 
    }
    bool usesGravity()  const override{
        return false; 
    }
    void move(Character* player, Level* level, float dt)   override;
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) {
        attack_timer += dt;
        if (attack_timer < 2.5f)
          return;
        attack_timer = 0.0f;

        float dx = player->getPosX() - pos_x;
        float abs_dx = (dx >= 0.0f) ? dx : -dx;
        if (abs_dx > 100.0f) {
            return;
        }
        Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y + (float)enemy_height, 0.0f, 4.5f, 20.0f, 16, 16);
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setSpriteTexture(g_tex);
        addToPool(p,pool,proj_count,max_proj);
  
    }

    void render(sf::RenderWindow& window, float cam_x, float cam_y) override {
        if (!is_alive) return;
        sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0,
            frame_width, frame_height));
        
        if (velocity_x >= 0.0f) {                          
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(pos_x - cam_x, pos_y - cam_y - (float)frame_height * 2.0f);
        }
        else {                                           
            sprite.setScale(-2.0f, 2.0f);
            sprite.setPosition(pos_x - cam_x, pos_y - cam_y - (float)frame_height * 2.0f);
        }
        window.draw(sprite);
    }
};
class M15ABradley : public Enemy {
private:
    float attack_timer;
    Texture rocket_tex_b;
    public:
    M15ABradley(float x, float y) {
        pos_x = x;
        pos_y = y;
        hp = 7.0f;
        speed = 0.35f;
        score_value = 250;
        attack_timer = 0.0f;
        frame_width = 119;
        frame_height = 125;
        texture.loadFromFile("Assets/m15_bradley.png");
        sprite.setTexture(texture);
       // sprite.setScale((float)enemy_width / (float)texture.getSize().x, (float)enemy_height / (float)texture.getSize().y);
        rocket_tex_b.loadFromFile("Assets/rocket.png");
        frame_width = 119; 
        frame_height = 125;   
        addClip(0, 10, 8.0f); 
    }
    bool usesGravity()  const override {
        return true;
    }
    void move(Character* player, Level* level, float dt)   override;
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) {
        attack_timer += dt;
        if (attack_timer < 3.0f)
           return;
        attack_timer = 0.0f;
        float dx = player->getPosX() - pos_x;
        float abs_dx = (dx >= 0.0f) ? dx : -dx;
        if (abs_dx > 950.0f) {
            return;
        }
        Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, (dx > 0.0f ? 5.0f : -5.0f), -11.0f,5.0f, 24, 12);
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setSpriteTexture(rocket_tex_b);
        addToPool(p, pool, proj_count, max_proj);
    }

    void render(sf::RenderWindow& window, float cam_x, float cam_y) override {
        if (!is_alive) return;
        sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0,
            frame_width, frame_height));

        if (velocity_x > 0.0f) {                              
            sprite.setScale(-2.0f, 2.0f);
            sprite.setPosition(pos_x - cam_x , pos_y - cam_y  -  (float)frame_height * 1.0f);
        }
        else {                                             
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(pos_x - cam_x, pos_y - cam_y  - (float)frame_height * 1.0f);
        }
        window.draw(sprite);
    }
};
class EnemySub : public Enemy {
private:
    float attack_timer, surface_timer, turn_timer;
bool surfacing;
float surface_target_y;
int last_dir;
float min_y;

Texture rocket_tex_b;
public:
    EnemySub(float x, float y) {
        pos_x = x;
        pos_y = y;
        attack_timer = 0.0f;
        surface_timer = 0.0f;
        surface_target_y = 0.0f;
        activation_range = 1400.0f;
        hp = 7.0f;
        min_y = y - 90.0f;
        surfacing = false;
        speed = 2.2f;
        score_value = 160;
        last_dir = 1;

       texture.loadFromFile("Assets/enemy_sub.png");
        sprite.setTexture(texture);
  //      sprite.setScale((float)enemy_width / (float)texture.getSize().x, (float)enemy_height / (float)texture.getSize().y);
        rocket_tex_b.loadFromFile("Assets/rocket.png");

        turn_timer = 0.0f;    
        frame_width = 194;    
        frame_height = 150;    
        addClip(0, 8, 8.0f);  
        addClip(8, 10, 10.0f);  


    }
    bool usesGravity()  const override {
        return false;
    }
    void move(Character* player, Level* level, float dt)  override;

    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) {
        attack_timer += dt;
        if (attack_timer < 2.5f)
          return;
        attack_timer = 0.0f;

        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;

        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < 1.0f) 
          dist = 1.0f;
        if (dist > 850.0f) 
          return;
        Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, (dx / dist) * 6.0f, (dy / dist) * 6.0f,
            5.0f, 24, 12);
        p->setFromEnemy(true); p->setExplosive(true, 3.0f);
        p->setSpriteTexture(rocket_tex_b);
        addToPool(p, pool, proj_count, max_proj);
    }
   
    
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override {
        if (!is_alive) return;
        sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0,
            frame_width, frame_height));

        float sc = (last_dir > 0) ? 3.0f : -3.0f;
        sprite.setScale(sc, 3.0f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y - (float)frame_height);
        window.draw(sprite);
    }

};

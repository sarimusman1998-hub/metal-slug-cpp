#pragma once
#include "Enemy.h"
#include "SFML/Graphics.hpp"
#include <cmath>

using namespace sf;

class IronNokana : public Enemy {
private:
    float missile_timer;
    float flame_timer;
    float fire_grenade_timer;
    bool  is_retreating;
    bool  has_retreated;


    Texture missile_tex_boss;
    Texture flame_tex_boss;
    Texture fire_grenade_tex_boss;



public:
    bool isRetreating() const { 
       return is_retreating; 
       }
    bool hasRetreated() const { 
       return has_retreated;
       }
    bool bypassesCollision() const override {
        return is_retreating;
    }
    IronNokana(float x, float y) {
        pos_x = x;
        pos_y = y;
        hp = 30.0f;
        speed = 1.2f;
        score_value = 500;
        enemy_width = 128;
        enemy_height = 90;
        power_boost = 1.0f;
        activation_range = 99999.0f;
        missile_timer = 0.0f;
        flame_timer = 0.0f;
        fire_grenade_timer = 0.0f;
        is_retreating = false;
        has_retreated = false;
        if (!texture.loadFromFile("25I-0555_25I-1034_Assets/iron_nokana.png"))
            texture.loadFromFile("25I-0555_25I-1034_Assets/m15_bradley.png");

        sprite.setTexture(texture);
        sprite.setScale( 2.5f, 2.5f);

        missile_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");
        flame_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/flame.png");
        fire_grenade_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");


        //fk abh is ke tex bhi sahi karne hein           fkkkk animation
        frame_width = 202;   
        frame_height = 140;  
        addClip(0, 4, 4.0f);
       }

    bool usesGravity() const override {
        return true;
    }
    int  getEnemyTypeID()  const override {
        return 8;
    }
    void onBlastHit(float dmg) override {
        takeDamage(dmg);
    }
    void move(Character* player, Level* level, float dt) override;
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) override;
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;

};

class HairbusterRiberts : public Enemy {
    float rocket_timer;
    float fire_grenade_timer;
    float patrol_timer;
    float patrol_dir;
    float hover_y;       
    bool  is_retreating;
    bool  has_retreated;

    Texture rocket_tex_boss;
    Texture fire_grenade_tex_boss;

public:
    bool isRetreating() const {
        return is_retreating;
    }
    bool hasRetreated() const {
        return has_retreated;
    }
    bool bypassesCollision() const override {
        return is_retreating;
    }
    HairbusterRiberts(float x, float y) {
        pos_x = x;
        pos_y = y;
        hp = 30.0f;
        speed = 2.5f;
        score_value = 500;
        enemy_width = 140;
        enemy_height = 80;
        power_boost = 1.0f;
        activation_range = 99999.0f;

        rocket_timer = 0.0f;
        fire_grenade_timer = 0.0f;
        patrol_timer = 0.0f;
        patrol_dir = 1.0f;

       hover_y = y;

       is_retreating = false;
       has_retreated = false;
       if (!texture.loadFromFile("25I-0555_25I-1034_Assets/hairbuster_riberts.png"))
           texture.loadFromFile("25I-0555_25I-1034_Assets/flying_tara.png");

       sprite.setTexture(texture);
       sprite.setScale(2.5f, 2.5f);
       rocket_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");
       fire_grenade_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");

       frame_width = 210;
       frame_height = 170;
       addClip(0, 45, 12.0f); // cool ass fly
       num_clips = 1;



    }

    bool usesGravity()    const override { 
      return false;
      }
    int  getEnemyTypeID() const override { 
        return 9;
        }
    void onBlastHit(float dmg) override { 
        takeDamage(dmg);
        }
    void move(Character* player, Level* level, float dt) override;
    void attack(Character* player, Projectile** pool, int& proj_count,int max_proj, Texture& bullet_tex, float dt) override;
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;
};

class SeaSatan : public Enemy {
private:
    float hmg_timer;
    float rocket_timer;
    float patrol_timer;
    float patrol_dir;
    bool  is_retreating;
    bool  has_retreated;

    Texture bullet_tex_boss;
    Texture rocket_tex_boss;


public:
    bool isRetreating() const {
        return is_retreating;
    }
    bool hasRetreated() const {
        return has_retreated;
    }
    bool bypassesCollision() const override {
        return is_retreating;
    }
    SeaSatan(float x, float y) {
        pos_x = x;
        pos_y = y;
        hp = 30.0f;
        speed = 1.5f;
        score_value = 500;
        enemy_width = 210;
        enemy_height = 170;
        power_boost = 1.0f;
        activation_range = 99999.0f;

        hmg_timer = 0.0f;
        rocket_timer = 0.0f;
        patrol_timer = 0.0f;
        patrol_dir = 1.0f;

        is_retreating = false;
        has_retreated = false;


        if (!texture.loadFromFile("25I-0555_25I-1034_Assets/sea_satan.png"))
            texture.loadFromFile("25I-0555_25I-1034_Assets/enemy_sub.png");

        sprite.setTexture(texture);
        sprite.setScale(  (float)enemy_width / (float)texture.getSize().x,(float)enemy_height / (float)texture.getSize().y );

        bullet_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/bullet.png");
        rocket_tex_boss.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");


        frame_width = enemy_width;    
        frame_height = enemy_height;   
        sprite.setScale(2.0f, 2.0f);  //  koinold scaling


    }
    bool usesGravity()    const override {
        return false;
    }
    int  getEnemyTypeID() const override {
        return 10;
    }
    void onBlastHit(float dmg) override {
        takeDamage(dmg);
    }
    void move(Character* player, Level* level, float dt) override;
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, Texture& bullet_tex, float dt) override;
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;
    void updateDamageFrame() {
// hp se sprite update ho jae     | 4 parts
        if (hp > 22.5f)
            current_frame = 0;
        else if (hp > 15.0f) 
            current_frame = 1;
        else if (hp > 7.5f) 
            current_frame = 2;
        else 
            current_frame = 3;
    }

};
class UltimateBoss : public Enemy {
private:
    float ground_hp;
    float aerial_hp;
    float aquatic_hp;

    int state;
    float state_timer;
    float state_duration;

    float hover_y;
    float water_y;

    bool all_defeated;

    float missile_timer_g;
    float flame_timer_g;
    float fire_grenade_timer_g;

    float rocket_timer_a;
    float fire_grenade_timer_a;
    float patrol_timer_a;
    float patrol_dir_a;

    float hmg_timer_q;
    float rocket_timer_q;
    float patrol_timer_q;
    float patrol_dir_q;

    Texture ground_tex;
    Texture aerial_tex;
    Texture aquatic_tex;

    Texture missile_tex;
    Texture flame_tex;
    Texture rocket_tex_u;
    Texture bullet_tex_u;
    Texture fg_tex;

    bool canUseState(int s) const {
        if (s == 0) 
           return ground_hp > 0.0f;
        if (s == 1)
            return aerial_hp > 0.0f;
        if (s == 2) 
           return aquatic_hp > 0.0f;
        return false;
    }
    void tryNextState() {
        int old = state;

        for (int i = 1;i <= 3;i++) {
          int newstate;
           newstate = (old+i) % 3;
           if (canUseState(newstate)) {
               state = newstate;
               velocity_y = 0.0f;
               state_duration = 10.0f + (float)(rand() % 6);
               state_timer = state_duration;
               return;
             }
         }

        is_alive = false;
        all_defeated = true;


    }

public:
    bool isDefeated() const {
        return all_defeated;
    }

    float getGroundHP() const {
        return ground_hp;
    }

    float getAerialHP() const {
        return aerial_hp;
    }

    float getAquaticHP() const {
        return aquatic_hp;
    }

    int getCurrentState() const {
        return state;
    }
    UltimateBoss(float x, float y, float h_y, float w_y) {
        pos_x = x;
        pos_y = y;
        hp = 90.0f;
        speed = 2.0f;
        score_value = 1500;
        enemy_width = 140;
        enemy_height = 90;
        power_boost = 1.0f;
        activation_range = 99999.0f;


        ground_hp = aerial_hp = aquatic_hp = 30.0f;
        state = 0;
        state_duration = 12.0f;
        state_timer = state_duration;
        hover_y = h_y;

        water_y = w_y;
        all_defeated = false;
        missile_timer_g = flame_timer_g = fire_grenade_timer_g = 0.0f;
        rocket_timer_a = fire_grenade_timer_a = patrol_timer_a = 0.0f;
        patrol_dir_a = 1.0f;
        hmg_timer_q = rocket_timer_q = patrol_timer_q = 0.0f;
        patrol_dir_q = 1.0f;
        if (!ground_tex.loadFromFile("25I-0555_25I-1034_Assets/iron_nokana.png")) {
            ground_tex.loadFromFile("25I-0555_25I-1034_Assets/m15_bradley.png");
        }

        if (!aerial_tex.loadFromFile("25I-0555_25I-1034_Assets/hairbuster_riberts.png")) {
            aerial_tex.loadFromFile("25I-0555_25I-1034_Assets/flying_tara.png");
        }

        if (!aquatic_tex.loadFromFile("25I-0555_25I-1034_Assets/sea_satan.png")) {
            aquatic_tex.loadFromFile("25I-0555_25I-1034_Assets/enemy_sub.png");
        }

        missile_tex.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");
        flame_tex.loadFromFile("25I-0555_25I-1034_Assets/flame.png");
        rocket_tex_u.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");
        bullet_tex_u.loadFromFile("25I-0555_25I-1034_Assets/bullet.png");
        fg_tex.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");

        texture = ground_tex;

        sprite.setTexture(texture);

        sprite.setScale(
            (float)enemy_width / (float)texture.getSize().x,
            (float)enemy_height / (float)texture.getSize().y
        );
    }
    bool usesGravity()    const override {
        return (state == 0);
    }
    int  getEnemyTypeID() const override {
        return 11;
    }
    void onBlastHit(float dmg) override {
        takeDamage(dmg);
    }
    void takeDamage(float dmg) override {
        was_attacked = true;
        if (state == 0) {
            ground_hp -= dmg;

            if (ground_hp < 0) {
                ground_hp = 0;
            }
        }
        else if (state == 1) {
            aerial_hp -= dmg;

            if (aerial_hp < 0) {
                aerial_hp = 0;
            }
        }
        else {
            aquatic_hp -= dmg;

            if (aquatic_hp < 0) {
                aquatic_hp = 0;
            }
        }

        hp = ground_hp + aerial_hp + aquatic_hp;
        if (!canUseState(state))
            tryNextState();

        if (ground_hp <= 0 && aerial_hp <= 0 && aquatic_hp <= 0) {
            is_alive = false;
            all_defeated = true;
        }


    }
    void move(Character* player, Level* level, float dt) override;
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bt, float dt) override;
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;

};


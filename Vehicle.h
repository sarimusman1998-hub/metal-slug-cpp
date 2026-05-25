#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"


class Character;
class Level;

class Vehicle {
protected:
    float pos_x, pos_y;
    float velocity_x, velocity_y;
    float hp, max_hp;
    float speed;
    int   width, height;
    bool  is_occupied;
    bool  is_alive;
    bool  on_ground;
    int jump_count;
    Character* driver;      // aggregation – vehicle does NOT own driver

    float fire_timer;
    float fire_cooldown;
    int   facing_dir;

    sf::Texture texture;

    int   current_frame;   //its 12 42 am on 11 may 
    int   frame_width;    
    int   frame_height;   
    float anim_timer;      

public:
    sf::Sprite sprite;

    Vehicle(float x, float y, float w, float h, float spd, float hp_val);
    virtual ~Vehicle();

    float getPosX() const;
    float getPosY() const;
    int   getWidth()  const;
    int   getHeight() const;
    float getHP()  const;
    float getMaxHP() const;
    bool  isAlive() const;
    bool  isOccupied() const;
    int   getFacingDir()const;

    bool isNearPlayer(Character* player, float range = 120.0f);
    
    void enter(Character* p);
    
    void exit();

    void takeDamage(float dmg);


    // same shit fkn 2 weeks later
    virtual void applyGravity(Level* level);
    void applyHorizontalCollision(Level* level);
    void moveLeft();
    void moveRight();
    void stopHorizontal();
    virtual void jump();
    virtual void moveUp() {

    }
    virtual void moveDown() {
    
    }
    virtual void stopVertical() {
    
    }
    virtual  void updateTimers(float dt) = 0;
    void Driver();      // keeps driver sprite position inside vehicle

    virtual void updateMode(Level* level) {
    
    }

    virtual Projectile* fire() = 0;
    virtual Projectile* fireMissile() {
       return nullptr;
       }
    virtual void cycleMissile() {
    
    }
    virtual void render(sf::RenderWindow& window, float cam_x, float cam_y);
};


// METALSLUG 

class MetalSlug : public Vehicle {
private:
    sf::Texture bullet_tex;
    float fire_flash_timer;               


public:
    MetalSlug(float x, float y);

    void moveUp() override { 
       jump(); 
       }

    void updateTimers(float dt) override; 

    Projectile* fire() override;
};

//SlugFlyer
class SlugFlyer : public Vehicle {
    Texture bullet_tex;
    Texture missile_tex;
    int missiles;
    float fire_flash_timer;                                           

public:
    SlugFlyer(float x, float y);
    void applyGravity(Level* level) override;   
    void moveUp()       override { 
    velocity_y = -speed * 0.8f;
    }
    void moveDown()     override { 
      velocity_y = speed * 0.8f; 
      }
    void stopVertical() override {
       velocity_y *= 0.85f; 
       }
    void jump()         override {
    
    }   
    Projectile* fire() override;
    Projectile* fireMissile() override; 
    void updateTimers(float dt) override;                              // ADDED
    void render(sf::RenderWindow& window, float cam_x, float cam_y) override; // ADDED – no facing_dir flip
};
//Slug Mariner
class SlugMariner : public Vehicle {
    Texture bullet_tex;
    Texture missile_tex;

    int missiles_h;
    int missiles_v;
    int missiles_r;
    int missile_mode;

public:
    SlugMariner(float x, float y);

    void applyGravity(Level* level) override;

    void moveUp() override {
        velocity_y = -speed * 0.7f;
    }

    void moveDown() override {
        velocity_y = speed * 0.7f;
    }

    void stopVertical() override {
        velocity_y *= 0.9f;
    }

    void jump() override {
    }

    void cycleMissile() override {
        missile_mode = (missile_mode + 1) % 3;
    }

    int getMissileMode() const {
        return missile_mode;
    }

    Projectile* fire() override;

    Projectile* fireMissile() override;

    void updateTimers(float dt) override;  



};

//Amphibious Slug
class AmphibiousSlug : public Vehicle {
    Texture ground_tex, fly_tex, marine_tex;
    Texture bullet_tex;
    Texture missile_tex;
    float fire_flash_timer;  // ADDED
    int missiles;
    int mode;

public:
    AmphibiousSlug(float x, float y);

    void updateMode(Level* level) override;

    void applyGravity(Level* level) override;
    void updateTimers(float dt)     override;  // ADDED
    void moveUp() override;

    void moveDown() override;

    void stopVertical() override;

    void jump() override;

    Projectile* fire() override;

    Projectile* fireMissile() override;

    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;
};
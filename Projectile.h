#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Level;

class Projectile {
protected:
    float damage, pos_x, pos_y, velocity_x, velocity_y, blast_radius,explode_x,explode_y, lifespan, lifespan_timer;;
    int proj_width;
    int proj_height;

    bool is_explosive, is_active,is_grenade,is_fire,just_exploded,is_from_enemy;
   
   public:
    Texture proj_tex;
    Sprite sprite;
    void setLifespan(float t) {
        if (t < 0) {
            return;
     }
       lifespan = t;
    }
    float getLifespan() {
        return lifespan;
    }
    float getLifespantimer() {
        return lifespan_timer;
    }
    float getPosX() const {
        return pos_x;
    }

    float getPosY() const {
        return pos_y;
    }

    float getVelocityX() const {
        return velocity_x;
    }

    float getVelocityY() const {
        return velocity_y;
    }

    float getDamage() const {
        return damage;
    }

    float getBlastRadius() const {
        return blast_radius;
    }
    float setBlastRadius(float rad) {
        if (rad < 0.0f) {
            return blast_radius;
    }
        blast_radius = rad;
    }
    float getExplodeX() const {
        return explode_x;
    }

    float getExplodeY() const {
        return explode_y;
    }

    int getWidth() const {
        return proj_width;
    }

    int getHeight() const {
        return proj_height;
    }

    bool isActive() const {
        return is_active;
    }
    void setActive(bool condition) {
        is_active = condition;
    }
    bool isExplosive() const {
        return is_explosive;
    }
    void setisExplosive(bool condition) {
        is_explosive = condition;
    }
    bool isGrenade() const {
        return is_grenade;
    }

    bool isFire() const {
        return is_fire;
    }
    void setisFire(bool condition) {
      is_fire = condition;
    }
    bool justExploded() const {
        return just_exploded;
    }

    bool isFromEnemy() const {
        return is_from_enemy;
    }
    void deactivate() {
        is_active = false;
    }

    void clearExploded() {
        just_exploded = false;
    }

    void addVelocityY(float dv) {
        velocity_y += dv;
    }

    void setFromEnemy(bool v) {
        is_from_enemy = v;
    }

    void setExplosive(bool v, float radius = 0.0f) {
        is_explosive = v;
        blast_radius = radius;
    }

    void setGrenade(bool v) {
        is_grenade = v;
    }

    void setFire(bool v) {
        is_fire = v;
    }

    void setSpriteTexture(sf::Texture& tex) {
        sprite.setTexture(tex);
    }

    void setSpriteScale(float sx, float sy) {
        sprite.setScale(sx, sy);
    }
    Projectile()  {
        pos_x = 0;
        pos_y = 0;

        velocity_x = 0;
        velocity_y = 0;

        damage = 3.0f;

        proj_width = 16;
        proj_height = 8;

        is_explosive = false;
        is_grenade = false;
        blast_radius = 0;

        is_active = false;

        is_fire = false;
        just_exploded = false;
        explode_x = 0.0f;
        explode_y = 0.0f;
        is_from_enemy = false;

        lifespan = 0.0f;
        lifespan_timer = 0.0f;


        proj_tex.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");
        sprite.setTexture(proj_tex);
        sprite.setPosition(pos_x, pos_y);
        sprite.setScale((float)proj_width / (float)proj_tex.getSize().x,(float)proj_height / (float)proj_tex.getSize().y);
    }





    // paramerterized constructor needed
    Projectile(float x, float y, float velocity_x, float velocity_y, float dmg, int w, int h) {
        pos_x = x;
        pos_y = y;

        this->velocity_x = velocity_x;
        this->velocity_y = velocity_y;

        damage = dmg;

        proj_width = w;
        proj_height = h;

        is_explosive = false;
        is_grenade = false;
        blast_radius = 0;

        is_active = true;

        is_fire = false;
        just_exploded = false;
        explode_x = 0.0f;
        explode_y = 0.0f;
        is_from_enemy = false;

        lifespan = 0.0f;
        lifespan_timer = 0.0f;

        proj_tex.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");
        sprite.setTexture(proj_tex);
        sprite.setPosition(pos_x, pos_y);
        sprite.setScale((float)proj_width / (float)proj_tex.getSize().x, (float)proj_height / (float)proj_tex.getSize().y);
    }
   virtual ~Projectile() {
   
   }  // manual delete in pool


    void update(Level* level);

    //void update(Level * level) {

    //    if (!is_active) return;

    //    pos_x += velocity_x;
    //    pos_y += velocity_y;

    //    if (pos_x < -200 || pos_x > level->width * level->cell_size + 200)
    //    {
    //        is_active = false;
    //        return;    // despan
    //    }
    //    if (pos_y < -200 || pos_y > level->height * level->cell_size + 200)
    //    {
    //        is_active = false;
    //        return;    // despan
    //    }

    //    // A    HIT  (war thunder players would know)
    //    int row = (int)(pos_y + proj_height / 2) / level->cell_size;
    //    int col = (int)(pos_x + proj_width / 2) / level->cell_size;

    //    char block = level->getBlock(row, col);

    //    if (block == 'g' || block == 'b') {
    //        is_active = false;

    //        if (is_explosive) {
    //            int radius_blocks = (int)blast_radius;

    //            for (int r = row - radius_blocks; r <= row + radius_blocks; r++)  // blocky radius , all gone reduced to atoms
    //            {
    //                for (int c = col - radius_blocks; c <= col + radius_blocks; c++)
    //                {
    //                    level->destroyBlock(r, c);
    //                }
    //            }


    //        }


    //    }




    //}//update end

    bool checkHit(float x2, float y2, int w2, int h2) {

        return (pos_x <= x2 + w2 && pos_x + proj_width >= x2 && pos_y <= y2 + h2 && pos_y + proj_height >= y2);
    }

    void render(RenderWindow& window, float cam_x, float cam_y) {

        if (!is_active) return;  // no harm wid dis 


        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
        window.draw(sprite);


    }
};
class StraightProjectile : public Projectile {
public:
    StraightProjectile(float x, float y, float vx, float vy,float dmg, int w, int h): Projectile(x, y, vx, vy, dmg, w, h) {
    
    }
};
class BallisticProjectile : public Projectile {
public:
    BallisticProjectile(float x, float y, float vx, float vy, float dmg, int w, int h) : Projectile(x, y, vx, vy, dmg, w, h) {
  
  }
};

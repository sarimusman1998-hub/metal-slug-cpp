#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyAIState.h" 
#include "Character.h"
#include "Projectile.h"
#include <cmath> 
#include <iostream>

class Character;
class Projectile;
class Level;

class Enemy  : public Character {
protected:
    int enemy_width;
    int enemy_height;
    int score_value;
    bool has_grudge,activated;
    float power_boost;      // attack when bac
    float activation_range;
    EnemyAIState* ai_state;
    bool was_attacked;
    struct AnimClip {
        int  start; // 0 ,4 , 16
        int count;
        float fps; // almost same but for odd shi like brad 7
    };

    static const int MAX = 10;     
    AnimClip clips[MAX];          
    int num_clips;     //how many now
    int  current_anim;    //  now

   public:
   Sprite grenade_sprite;
   Texture grenade_tex;
   Sprite rocket_sprite;
   Texture rocket_tex;
    // Anime   fk kitni dafa kabhi hatao phir banao phir mfkr ye koi course karwate is shit ka faaahahahahahhahhhhhh
   // whyyyyy
   void addClip(int start, int count, float fps) {     
       if (num_clips < MAX) {
           clips[num_clips].start = start;
           clips[num_clips].count = count;
           clips[num_clips].fps = fps;
           num_clips++;
       }
   }

   void playAnim(int index) {                          // getter
       if (index >= 0 && index < num_clips)
           current_anim = index;
   }
   void setPowerBoost(float boost) {
       power_boost = boost;
   }

   void forceRevive(float new_hp) {
       hp = new_hp;
       is_alive = true;
   }
   float getHP() const { 
      return hp; 
      }
    
    void checkAndJump(Level* level);
    void applyHorizontalCollision(Level* level);
    void activateSpecial() override {}
    virtual void move(Character* player, Level* level, float dt) = 0;
    virtual void attack(Character* player,Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) = 0;
    virtual bool usesGravity() const { return true; }
    virtual bool bypassesCollision() const { return false; }
    virtual void onBlastHit(float dmg) {
        takeDamage(dmg);
    }
    virtual int getEnemyTypeID() const {
        return -1;
    }
    virtual bool blocksBullet(float proj_vx, float shooter_x) const {
        return false;
    }
    virtual bool handleSpecialHit(Projectile* p) {
        return false;
    }
    virtual bool shouldBeSweepable() const {
       return true; 
       }
    bool checkCollision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2) {
        return (x1 <= x2 + w2 && x1 + w1 >= x2 && y1 <= y2 + h2 && y1 + h1 >= y2);
    }
    int getEnemyWidth() const {
        return enemy_width;
    }

    int getEnemyHeight() const {
        return enemy_height;
    }
    int getScorevalue()const {
        return score_value;
    }
    bool isEnemyAlive() const {
        return is_alive;
    }
    bool get_wasattcked() {
        return was_attacked;
    }
    Enemy()
    {
       
        
        enemy_width = 50;
        enemy_height = 60;
        score_value = 50;
       
        

        was_attacked = false;
        has_grudge = false;
        power_boost = 1.0f;
        activated = false;
        activation_range = 850.0f;
        ai_state = new DefaultAIState();


       grenade_tex.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");
       grenade_sprite.setTexture(grenade_tex);

       rocket_tex.loadFromFile("25I-0555_25I-1034_Assets/rocket.png");
       rocket_sprite.setTexture(rocket_tex);

       num_clips = 0;                          //anime
       for (int i = 0; i < MAX; i++) {
         clips[i].start = 0; 
         clips[i].count = 1;
         clips[i].fps = 6.0f;
         }
       current_anim = 0;
    }
    virtual ~Enemy()
    {
        if (ai_state)
          delete ai_state;
    }

    void takeDamage(float dmg)
    {
        hp -= dmg;
        was_attacked = true;
        if (hp <= 0) 
          is_alive = false;
    }

    void applyGravity(Level* level);
    //void applyGravity(Level* level) {
    //    
    //    // Same gravity different race   || no racisim allowed  but simpler

    //    const float gravity = 0.5f;
    //    const float terminal_vel = 10.0f;
    //    int cs = level->cell_size;

    //    velocity_y += gravity;

    //    velocity_y > terminal_vel? velocity_y = terminal_vel : velocity_y ;

    //    float next_y = pos_y + velocity_y;

    //    int foot_row = (int)(next_y + enemy_height) / cs;
    //    int col = (int)(pos_x + enemy_width / 2) / cs;

    //    if ( ( foot_row < 0 || foot_row >= level->height ) || ( col < 0 || col >= level->width ) )  // why are u runnin
    //    {
    //        on_ground = true;
    //        velocity_y = 0;
    //        return;
    //    }

    //    char block = level->getBlock(foot_row, col);

    //    if ( ( block == 'g' || block == 's' || block == 'b' ) && velocity_y > 0 )
    //    {
    //        on_ground = true;
    //        velocity_y = 0;
    //        pos_y = foot_row * cs - enemy_height;
    //    }
    //    else
    //    {
    //        pos_y = next_y;
    //        on_ground = false;
    //    }
    //}


void update( Character* player, Level* level, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) // fkn kitni dafa change karna pare ga
 {

    if (!is_alive)  // dead mfkr
        return;

    if (!activated) {
        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;

        if (dx * dx + dy * dy > activation_range * activation_range) {
            if (usesGravity())
            applyGravity(level);
            //animateSprite(sprite, animation_timer, current_frame, 0, total_frames, 12.0f, frame_width, frame_height, dt); 
            if (num_clips > 0)                      // CHANGED: use clip 0 (idle) when inactive
            {
                animateSprite(sprite, animation_timer, current_frame, clips[0].start, clips[0].count, clips[0].fps, frame_width, frame_height, dt);
            }
            return;
        }
        activated = true;
    }
    current_anim = 0;  // def

    ai_state->execute(this, player, level, pool, proj_count, max_proj, bullet_tex,dt);
    if (!bypassesCollision())
        applyHorizontalCollision(level);

        if (usesGravity())
        {
            applyGravity(level);
        }

        // Animation
        
        //animation_timer += dt;
        
        if (num_clips > 0)  //run anime
            animateSprite(sprite, animation_timer, current_frame,clips[current_anim].start, clips[current_anim].count,clips[current_anim].fps, frame_width, frame_height, dt);


    }

virtual void render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive)  // dead mfkr
        return;

    sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));  //  jo bhi tutorial ddekhe wo to rectangle aur circle se kare hum kyun nahi allowed

    sprite.setPosition(pos_x - cam_x, (pos_y - cam_y + (float)frame_height - (float)frame_height * 2.0f) + 20.0f);


    if (velocity_x <= 0) {
        sprite.setScale(2.0f, 2.0f);
    }
    else {

        sprite.setScale(-2.0f, 2.0f);
    }
    window.draw(sprite);
}

};



class RebelSoldier: public Enemy  {

protected:

    float attack_timer, attack_cooldown;
     const int IDLE = 0;               
     const int WALK = 1;              // good parctice    aaa maam ne kaha tha 
     const int SHOOT = 2;
public:
    RebelSoldier(float x, float y) 
    {
        hp = 2;

        pos_x = x;
        pos_y = y;
        speed = 1.5f;
        
        score_value = 50;

        attack_timer = 0;
        attack_cooldown = 1.0f;     

        texture.loadFromFile("25I-0555_25I-1034_Assets/rebel_soldier.png");
        sprite.setTexture(texture);
		frame_height = 57;
        frame_width = 59;
		total_frames = 26;


        //ANIME
        addClip(0, 4, 6.0f);                     //idle
        addClip(4, 12, 12.0f);                     //walk
        addClip(16, 10, 12.0f);                     //shoot
    }

    void move(Character* player,Level *level, float dt)
    {
        playAnim(WALK);   // WALK U NIGR

        // AI (the old type)
        float effective_speed = speed * power_boost;
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;



        checkAndJump(level);
    }

    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
        attack_timer += dt;
        if (attack_timer < attack_cooldown) // why dafuk does this not work wid ternary???
        {
            return;
        }

        attack_timer = 0.0f;
        playAnim(SHOOT);   // WALK U NIGR


        // Fire a bullet toward the player
        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 1) dist = 1;

        if (dist > 600)
              return;  // range is 600 px


        float bullet_speed = 5.0f;

        float vx = (dx / dist) * bullet_speed;
        float vy = (dy / dist) * bullet_speed;


      Projectile* p = new Projectile(pos_x + 20, pos_y + 20, vx, vy, 3.0f, 12, 8);
        p->sprite.setTexture(bullet_tex);
        p->sprite.setScale(3.5f, 3.5f);
        p->setFromEnemy(true);
        p->setLifespan(6.0f);
        for (int i = 0; i < max_proj; i++)
        {
            if (pool[i] == 0 || !pool[i]->isActive())
            {
                if (pool[i]) delete pool[i];
                pool[i] = p;
                return;
            }
        }
        delete p;
    }// attack end
    int getEnemyTypeID() const {
        return 0;
    }
};


class ShieldedSoldier : public Enemy {
protected:
    bool shield_active;
    float attack_timer;
    int target_facing;
    int shield_facing;
    float shield_turn_timer;

    static const int IDLE = 0;
    static const int WALK = 1;              // good parctice    aaa maam ne kaha tha 
    static const int TURN = 2;  
    static const int SHOOT_WALK = 3;
    //   static const int SHOOT = 2;


public:
    bool getShieldActive() {
        return shield_active;
    }
    ShieldedSoldier(float x, float y) 
    {
        hp = 5;
        pos_x = x;
        pos_y = y;
        score_value = 75;
        target_facing = 1;
        shield_active = true; // js check later if sheilded
        attack_timer = 0;
        speed = 1.0f;
		frame_height = 50;
        frame_width = 50;
        texture.loadFromFile("25I-0555_25I-1034_Assets/shielded_soldier.png");
        sprite.setTexture(texture);
        //ANIME
        addClip(0, 6, 6.0f);                     //idle
        addClip(6, 12, 12.0f);                     //walk
        addClip(18, 5, 8.0f);    // turn               
        addClip(23, 10, 12.0f);   // shooty walk kabhi rukte nahi hein 
    }

    void move(Character* player, Level* level, float dt)
    {
        if (shield_turn_timer > 0.0f)
            playAnim(TURN);        // CHANGED
        else
            playAnim(WALK);   // WALK U NIGR

        int player_side = (player->getPosX() < pos_x) ? 0 : 1;

        if (shield_turn_timer > 0.0f) {
            shield_turn_timer -= dt;
            if (shield_turn_timer <= 0.0f)
                shield_facing = target_facing;  
        }
        else if (player_side != shield_facing) {
            target_facing = player_side;  
            shield_turn_timer = 3.0f;   
        }

        float effective_speed = speed * power_boost;

        // AI (the old type)
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;

        checkAndJump(level);
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
        playAnim(SHOOT_WALK);  //  
        attack_timer += dt;
        if (attack_timer < 1.5f) // why dafuk does this not work wid ternary???
        {
            return;
        }
        attack_timer = 0;


        // Fire a bullet toward the player
        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;
        float dist = sqrt(dx * dx + dy * dy);
        dist < 1 ? dist = 1 : dist;

        if (dist > 500) return;  // range is 500 px


        float bullet_speed = 4.0f;

        float vx = (dx / dist) * bullet_speed;
        float vy = (dy / dist) * bullet_speed;

        Projectile* p = new Projectile(pos_x + 20, pos_y + 20, vx, vy, 3.0f, 12, 8);
        p->setFromEnemy(true);
        p->sprite.setTexture(bullet_tex);
        p->sprite.setScale(3.5f, 3.5f);
        p->setLifespan(6.0f);

        for (int i = 0; i < max_proj; i++)
        {
            if (pool[i] == 0 || !pool[i]->isActive())
            {
                if (pool[i]) delete pool[i];
                pool[i] = p;
                return;
            }
        }
        delete p;



    }// attack end
    bool blocksBullet(float proj_vx, float shooter_x) const override { 
        if (!shield_active)
            return false;
        if (shield_facing == 0) 
          return (proj_vx > 0 && shooter_x < pos_x);
        else                  
         return (proj_vx < 0 && shooter_x > pos_x);
    }
    int getEnemyTypeID() const {
        return 1;
    }
};
class BazookaSoldier : public Enemy {
protected:
    float attack_timer;
    float dive_timer;   // fkr ko dive hi karni thi mf
    bool  dive_done;    
    static const int IDLE = 0;
    static const int WALK = 1;              // good parctice    aaa maam ne kaha tha 
    static const int DIVE = 2;  // dave ko maro
    static const int SHOOT = 3;  // shooooot
public:
    BazookaSoldier(float x, float y) 
    {
        hp = 2;
        pos_x = x;
        pos_y = y;
       
        score_value = 100;
        speed = 0.8f;
        
        attack_timer = 0;

        texture.loadFromFile("25I-0555_25I-1034_Assets/bazooka_soldier.png");
        sprite.setTexture(texture);
        frame_width = 62;
        frame_height = 45;
        dive_timer = 0.0f;
        dive_done = false;
        //ANIME
        addClip(0, 4, 6.0f);                     //idle
        addClip(4, 12, 12.0f);                     //walk
        addClip(18, 8, 10.0f);   // dive  and he saves the gooooooooaaaaaaalllllllllllllllllllllll   
        addClip(26, 16, 12.0f);   // shoot  
    }
    void move(Character* player, Level* level, float dt)
    {
        playAnim(WALK);   // WALK U NIGR

        // Slow NIGGA
        float effective_speed = speed * power_boost;
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;
        checkAndJump(level);
    }
    
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) {
    
        attack_timer += dt;
        if (attack_timer < 3.0f)
        {
            if (attack_timer >= 2.5f) playAnim(DIVE);
            
            return;
        }    
        attack_timer = 0;

        playAnim(SHOOT);   // SHOOT after dave

        float dx = player->getPosX() - pos_x;
        float abs_dx = (dx >= 0.0f) ? dx : -dx; 
        if (abs_dx > 800.0f) 
          return;


        //  ballistic mf is so weird
        float vx = (dx > 0) ? 3.0f : -3.0f;
        float vy = -8.0f;   // upward arc

        Projectile* p = new Projectile(pos_x + 20, pos_y, vx, vy, 5.0f, 20, 10);
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->sprite.setTexture(rocket_tex);  // rocket tex where? 
    

        for (int i = 0; i < max_proj; i++)
        {
            if (pool[i] == 0 || !pool[i]->isActive())
            {
                if (pool[i]) delete pool[i];
                pool[i] = p;
                return;
            }
        }
        delete p;

    }//attack end
    int getEnemyTypeID() const {
        return 2;
    }
};
class GrenadeSoldier : public Enemy {
protected:
    float attack_timer;

     const int IDLE = 0;
     const int WALK = 1;              // good parctice    aaa maam ne kaha tha 
     const int SHOOT = 2;
    public:
    GrenadeSoldier(float x, float y) 
    {
        pos_x = x;
        pos_y = y;
        hp = 2;
        score_value = 100;
        speed = 1.2f;
        attack_timer = 0;

        texture.loadFromFile("25I-0555_25I-1034_Assets/grenade_soldier.png");
        sprite.setTexture(texture);
        frame_width = 50;
		frame_height = 42;
        //ANIME
        addClip(0, 4, 6.0f);                     //idle
        addClip(4, 12, 12.0f);                     //walk
        addClip(16, 14, 12.0f);                     //shoot

    }


    void move(Character* player, Level* level, float dt)
    {
        playAnim(WALK);   // WALK U NIGR

        float effective_speed = speed * power_boost;
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;
        checkAndJump(level);
    }


    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
        playAnim(SHOOT);   // SHOOT
        attack_timer += dt;
        
        if (attack_timer < 2.0f) return;
       
        attack_timer = 0;
        float dx = player->getPosX() - pos_x;
        float abs_dx = (dx >= 0.0f) ? dx : -dx; // can fire both dir
        if (abs_dx > 600.0f) return;

        //arc bark arch
        float vx = (dx > 0) ? 4.0f : -4.0f;
        float vy = -7.0f;

        Projectile* p = new Projectile(pos_x + 20, pos_y, vx, vy, 20.0f, 16, 16);
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->sprite.setTexture(grenade_tex);

        for (int i = 0; i < max_proj; i++)
        {
            if (pool[i] == 0 || !pool[i]->isActive())
            {
                if (pool[i]) delete pool[i];
                pool[i] = p;
                return;
            }
        }
        delete p;   

    }//attack end
    int getEnemyTypeID() const {
        return 3;
    }
};
class MummyWarrior : public Enemy {
protected:
    float resurrection_timer;
    bool crumbling;
    static const int IDLE = 0;
    static const int WALK = 1;              // good parctice    aaa maam ne kaha tha 
    //static const int SHOOT = 2;

    public:
    MummyWarrior(float x, float y) 
    {

        hp = 5;

        pos_x = x;
        pos_y = y;
        speed = 0.6f;
        
        score_value = 150;

        resurrection_timer = 0;

        crumbling = false;
        frame_width = 35;
        frame_height = 45;
        texture.loadFromFile("25I-0555_25I-1034_Assets/mummy.png");
        sprite.setTexture(texture);
        //ANIME
        addClip(0, 4, 6.0f);                     //idle
        addClip(4, 18, 12.0f);                     //walk


    }

    void move(Character* player, Level* level, float dt)
    {
        playAnim(WALK);   // WALK U NIGR


        if (crumbling)
        {

            resurrection_timer += dt;
            if (resurrection_timer >= 3.0f)
            {
                crumbling = false;
                resurrection_timer = 0;
                hp = 5;     // ARISE MY SOILDERS
            }
            return;
        }

        float effective_speed = speed * power_boost;
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;

        if (checkCollision(pos_x, pos_y, enemy_width, enemy_height, player->getPosX(), player->getPosY(), (int)player->getCharacterWidth(), (int)player->getCharacterHeight()))
        {
            if (!player->isDevImmortal() && !player->isInVehicle())
            player->setTransformationState(new MummyTransformation(), 10.0f);
        }

        checkAndJump(level);
    }
    
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
    // like epstine only touching no guns or swords    ||   hehehehehehehdhehehe  no shooy a nime
    }
    void render(sf::RenderWindow& window, float cam_x, float cam_y) 
    {
        if (!is_alive) return;
        if (crumbling) return;   

        sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));
        sprite.setPosition(pos_x - cam_x, (pos_y - cam_y + (float)frame_height - (float)frame_height * 2.0f) + 20.0f);


        if (velocity_x <= 0) {
            sprite.setScale(2.0f, 2.0f);
        }
        else {

            sprite.setScale(-2.0f, 2.0f);
        }
        window.draw(sprite);
    }
    void onBlastHit(float dmg) override {           
        is_alive = false;
    }
    bool handleSpecialHit(Projectile* p) override {  
        if (p->isFire()) { 
        is_alive = false; 
        return true;
      }
        if (!crumbling && !p->isExplosive() && p->getDamage() < 100) {
            crumbling = true;
            resurrection_timer = 0;
            return true;
        }
        return false;
    }
    int getEnemyTypeID() const {
        return 5;
    }
    bool shouldBeSweepable() const override {
      return !crumbling;
      }

};


class Zombie : public Enemy {
protected:
    float attack_timer;
    // const int IDLE = 0;
     const int WALK = 0;              // good parctice    aaa maam ne kaha tha 
   //  const int SHOOT = 2;
public:
    Zombie(float x, float y)
    {
        hp = 5;

        pos_x = x;
        pos_y = y;
        speed = 0.8f;

        score_value = 100;

        attack_timer = 0;

        texture.loadFromFile("25I-0555_25I-1034_Assets/zombie.png");
        sprite.setTexture(texture);
		frame_height = 50;
		frame_width = 40;
        //ANIME
     //   addClip(0, 4, 6.0f);                     //idle
        addClip(0, 12, 12.0f);                     //walk
       // addClip(16, 10, 12.0f);                     //shoot
    }

    void move(Character* player, Level* level, float dt)
    {
        playAnim(WALK);   // WALK U NIGR

        float effective_speed = speed * power_boost;
        if (player->getPosX() < pos_x)
            velocity_x = -effective_speed;
        else
            velocity_x = effective_speed;

        if (checkCollision(pos_x, pos_y, enemy_width, enemy_height, player->getPosX(), player->getPosY(), (int)player->getCharacterWidth(), (int)player->getCharacterHeight()))
        {
            if (!player->isDevImmortal() && !player->isInVehicle())
            player->setTransformationState(new ZombieTransformation(), 10.0f);
        }
        checkAndJump(level);
    }
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
    
        attack_timer += dt;

        if (attack_timer < 2.0f) return;
        attack_timer = 0;
     //   playAnim(SHOOT);   // SHOOT

        float dx = player->getPosX() - pos_x;
        float dy = player->getPosY() - pos_y;

        float dist = sqrt(dx * dx + dy * dy);

        if (dist < 1) dist = 1;
        if (dist > 400) return;



        float vx = (dx / dist) * 3.0f; // slow bullets to change the shi up
        float vy = (dy / dist) * 3.0f;

        Projectile* p = new Projectile(pos_x + 20, pos_y + 20, vx, vy, 3.0f, 12, 8);
        p->setFromEnemy(true);
        p->sprite.setTexture(bullet_tex);

        for (int i = 0; i < max_proj; i++)
        {
            if (pool[i] == 0 || !pool[i]->isActive())
            {
                if (pool[i]) delete pool[i];
                pool[i] = p;
                return;
            }
        }
        delete p;

    }//attack end
    int getEnemyTypeID() const {
        return 6;
    }
};
class Martian : public Enemy {
protected:
    int phase;          // 1 or 2
    bool pod_active;
    float attack_timer;
    float fly_height;
    float patrol_timer;
    float patrol_dir;
    bool fly_height_initialized;

    // const int IDLE = 0;
     const int WALK = 0;              // good parctice    aaa maam ne kaha tha 
     const int FLY = 1;
    public:
    Martian(float x, float y)
    {
        hp = 3;
        pos_x = x;
        pos_y = y;
        
        score_value = 200;
        phase = 1;
        pod_active = true;
        patrol_timer = 0.0f;
        patrol_dir = 1.0f;
        attack_timer = 0;
        fly_height = y;     // remember starting height
        speed = 2.0f;

        texture.loadFromFile("25I-0555_25I-1034_Assets/martian.png");
        sprite.setTexture(texture);
        frame_height = 60;
        frame_width = 60;
        fly_height_initialized = false;

        //ANIME
        addClip(0, 16, 12.0f);                     //walk
        addClip(16, 16, 12.0f);                      //fly u fkr
    }
    bool usesGravity() const override { return (phase == 2); }
    void move(Character* player, Level* level, float dt);
    void attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt)
    {
       // playAnim(SHOOT);   // SHOOT

        attack_timer += 1.0f / 60.0f;
        if (phase == 1) 
        {

            float dx = player->getPosX() - pos_x;
            float abs_dx = (dx >= 0.0f) ? dx : -dx; 
            if (abs_dx > 80.0f) return;     // only fire staight down
            
            if (attack_timer < 1.0f) return;
            
            attack_timer = 0;

            Projectile* p = new Projectile(pos_x + 20, pos_y + 40, 0, 8.0f, 5.0f, 8, 40);
            p->setFromEnemy(true);
           p->sprite.setTexture(bullet_tex);

                
            for (int i = 0; i < max_proj; i++)
            {
                if (pool[i] == 0 || !pool[i]->isActive())
                {
                    if (pool[i]) delete pool[i];
                    pool[i] = p;
                    return;
                }
            }
            delete p;

        }

        else  // phase 2  pistol whip out kar lia
        {
            if (attack_timer < 1.5f) return;
            attack_timer = 0;

            float dx = player->getPosX() - pos_x;
            float dy = player->getPosY() - pos_y;

            float dist = sqrt(dx * dx + dy * dy);
          
			dist < 1 ? dist = 1 : dist;
           
            if (dist > 500)
               return;

            float vx = (dx / dist) * 4.0f;
            float vy = (dy / dist) * 4.0f;

            Projectile* p = new Projectile(pos_x + 20, pos_y + 20,  vx, vy, 3.0f, 12, 8);
          p->setFromEnemy(true);
           p->sprite.setTexture(bullet_tex);

            for (int i = 0; i < max_proj; i++)
            {
                if (pool[i] == 0 || !pool[i]->isActive())
                {
                    if (pool[i]) delete pool[i];
                    pool[i] = p;
                    return;
                }
            }
            delete p;

        }
    
    }//attack end
    int getEnemyTypeID() const {
        return 7;
    }
    void takeDamage(float dmg) override {
        was_attacked = true;
        if (phase == 1) {
            hp -= dmg;
            if (hp <= 1.0f) {
                phase = 2;
                pod_active = false;
                hp = 2.0f;   
            }
        }
        else {
            hp -= dmg;
            if (hp <= 0)
              is_alive = false;
        }
    }
    // finally done damn too many enemies
};

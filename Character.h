#pragma once
#include "Weapon.h"
#include "DamagableEntity.h"  
#include "TransformationState.h"
#include "Scoring_Bonuses.h"
#include"Animation.h"
#include <SFML/Graphics.hpp>
//been fixing this file since 3AM
class Enemy;
class Level;
class Projectile;
class Weapon;
class Scoring;
//class Grenade;

class Character : public DamagableEntity {

protected:
    float hp;
    float speed, pos_x, pos_y, character_width, character_height;
    int lives;
    bool  is_alive;

    int grenade_count, facing_direction;      // 0 = left, 1 = right
    bool in_vehicle;
    float aim_angle;



    //dmg
    int damage_state;   // 3 states 0,1,2 norm dmg crit
    float damage_timer; // seconds

    // State 
    int  character_state;   // normal, undead, mummy  
    TransformationState* transform_state;
    float state_timer; // how long le effect is
    float grenade_cooldown_timer = 0.0f;

    //hunger
    int saturation;
    bool is_fat;


    float velocity_x;
    float velocity_y;
    bool on_ground;
    int jump_count;
    float jump_buffer;
    float double_jump_cooldown;

    Weapon* current_weapon;  // from wepon class point to it aggr

    // Grenades
    //Grenade** grenades;    // fron grenade class point to it but composite   . pointer to an array of grnds  
    int max_grenades,old_grenade_count;


    // Special
    bool special_active;    // is any abitlity active
    float special_timer;


   float melee_timer ;
   float melee_cooldown ;

   bool is_invincible ;
   float invincible_timer ;

    
    // SFML rndr




    //animation 
    int current_frame;
    float animation_timer;
    int frame_width;
    int frame_height;
    int total_frames;

    //Water
    bool in_water;


    //Dev Mode
    bool is_dev;

    //Grenade
    float grenade_blast_radius;

public:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture grenade_tex;
    sf::Sprite grenade_sprite;

    float getPosX() {
        return pos_x;
    }

    float getPosY() {
        return pos_y;
    }

    float getCharacterWidth() {
        return character_width;
    }

    float getCharacterHeight() {
        return character_height;
    }
    float getSpeed() const { 
      return speed; 
      }
    float getHP() {
        return hp;
    }

    bool isAlive() {
        return is_alive;
    }
    void setGrenadeCount(int n) {
        grenade_count = n;
    }
    int get_old_grenade_count() {
        return old_grenade_count;
    }
    void setHP(float new_hp) {
        hp = new_hp;
        if (hp <= 0.0f)
           is_alive = false;
    }

    int getLives() {
        return lives;
    }
    int setLives(int x) {
        if (x > 3 || x < 0) {
            return lives;
    }
        this->lives = x;
    }
    bool getOnground() {
        return on_ground;
    }
    bool isDevImmortal() const { 
      return is_dev;
      }
    int getDamageState() {
        return damage_state;
    }

    int getGrenadeCount() {
        return grenade_count;
    }
    void addSaturation(int n) { 
       saturation += n;
         if (saturation > 20) 
         saturation = 20; 
      }
    int  getSaturation() const {
       return saturation; 
       }
    int getFacingDirection() {
        return facing_direction;
    }
    bool isInVehicle() const {
        return in_vehicle;
    }
    void setInVehicle(bool v) {
        in_vehicle = v;
    }
    float getAimAngle() {
        return aim_angle;
    }

    Weapon* getCurrentWeapon() {
        return current_weapon;
    }
    void setPosition(float x, float y) {
        pos_x = x;
        pos_y = y;
    }

    void setAimAngle(float a) {
        aim_angle = (a < 0.0f) ? 0.0f : (a > 90.0f) ? 90.0f : a;
    }

    void resetMovement() {
        velocity_x = 0;
        velocity_y = 0;
        on_ground = false;
        jump_count = 0;
    }

    void resetState() {
        lives = 2;
        is_alive = true;
        damage_state = 0;
        damage_timer = 0.0f;
        is_invincible = false;
        character_state = 0;
    }
    bool isInWater() const {
        return in_water;
    }
    void setDevImmortal(bool condition) {
        is_dev = condition;
    }

    void swimUp() {
        if (in_water)
           velocity_y -= 10.0f;
    }
    void sink() {
        if (in_water)
            velocity_y += 5.0f;
    }
    //Special
    virtual void activateSpecial() = 0;
    virtual void onWeaponAssigned() {

    }
    virtual bool canPierceShield() const {
        return false;
    }
    // constructor
    Character() {

        hp = 100.0f;
        lives = 2;
        is_alive = true;


        speed = 4.0f;
        pos_x = 200.0f;
        pos_y = 500.0f;
        character_width = 60;           //anime
        character_height = 90;

        grenade_count = 10;
        old_grenade_count = grenade_count;

        jump_buffer = 0.0f;
        double_jump_cooldown = 0.4f;
        jump_count = 0;
        facing_direction = 1;
        in_vehicle = false;
        aim_angle = 0.0f;

        damage_state = 0;
        damage_timer = 0.0f;

        character_state = 0;   //normal or mommy or unded
        state_timer = 0.0f;
        transform_state = new NormalTransformation();

        saturation = 0;
        is_fat = false;     //caseoh

        velocity_x = 0;
        velocity_y = 0;
        on_ground = false;

        grenade_cooldown_timer = 0.0f;

      current_weapon = nullptr;   //wp
    //  grenades = nullptr;         //grnds
        max_grenades = 10;




        special_active = false;    // you are my special
        special_timer = 0.0f;     // gojo     fans



        melee_timer = 0.0f;
        melee_cooldown = 0.5f;

        is_invincible = false;          // are u sure?    i made a steak......... a steak!!!!!!????
        invincible_timer = 0.0f;                      

       
        
        current_frame = 0;      // one frame man
        animation_timer = 0.0f;
        frame_width = 64;
        frame_height = 64;
        total_frames = 1;


        in_water = false;
        is_dev = false;
        grenade_blast_radius = 3.0f;

       
    }
    void setTransformationState(TransformationState* new_state, float duration) {
        if (transform_state) delete transform_state;
        transform_state = new_state;
        character_state = new_state->getStateID();
        state_timer = duration;
    }
    //destructor  (beerus)
    virtual ~Character()
    {
        /*if (grenades) {
            for (int i = 0; i < max_grenades; i++)
            {

                if (grenades[i])
                {
                    delete grenades[i];
                }

            }

            delete[] grenades;

        }*/


        if (transform_state)
           delete transform_state;


    }


    virtual  void moveLeft(float dt){//ANIME VIRTUALLY E
        velocity_x = -speed * transform_state->getSpeedMultiplier();   
        facing_direction = 0;//L
        animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);//anime

    }

    virtual  void moveRight(float dt){//ANIME VIRTUALLY E

        velocity_x = speed * transform_state->getSpeedMultiplier();
        facing_direction = 1;//R
        animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);//anime
    }
    



    void stopHorizontal()       // for collison
    {
        velocity_x = 0;
    }

    virtual  void jump(float dt){//ANIME VIRTUALLY E
        if (on_ground)
        {
            jump_count = 0;
        }
       if (jump_buffer <= 0 && on_ground)
        {
           velocity_y = -13.0f;
           on_ground = false;
           jump_count = 1;
           jump_buffer = double_jump_cooldown;
           animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);//anime

        }
       else if (!on_ground && jump_count == 1 && jump_buffer <= 0) {
           velocity_y = -10.0f;
           jump_count = 2;
           jump_buffer = double_jump_cooldown;
           animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);//anime

       }
    }

    //NEW IDLE FOR ANIME
    virtual void playIdleAnim(float dt) {
        animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
    }

    void setWeapon(Weapon* w) {
        current_weapon = w;
        onWeaponAssigned();
    }
    void applyGravity(Level* level);
    //void applyGravity(Level* level)
    //{
    //    const float gravity = 0.5f;
    //    const float terminal_vel = 10.0f;
    //    int cs = level->cell_size;    // take frm lvl



    //    // Vert

    //    velocity_y += gravity;

    //    velocity_y > terminal_vel? velocity_y = terminal_vel : velocity_y;  //no bs infinite speed and acc



    //    float next_y = pos_y + velocity_y;

    //    // Check block below feet
    //    int foot_row = (int)(next_y + character_height) / cs;
    //    int col_left = (int)(pos_x) / cs;
    //    int col_mid = (int)(pos_x + character_width / 2) / cs;
    //    int col_right = (int)(pos_x + character_width) / cs;



    //    //bounds
    //    if (foot_row < 0 || foot_row >= level->height) {  // no escape from za wardo
    //        on_ground = true;
    //        velocity_y = 0;
    //        return;
    //    }

    //    col_left < 0 ? col_left = 0 : col_left;
    //    col_right >= level->width ? col_right = level->width - 1 : col_right;   // no teleportaion or loop on the boundaries
    //    col_mid < 0 ? col_mid = 0 : col_mid;
    //    col_mid >= level->width ? col_mid = level->width - 1 : col_mid;             // 3 point mth idk i remember from THE TUMBLE 


    //    char block_left = level->getBlock(foot_row, col_left);
    //    char block_mid = level->getBlock(foot_row, col_mid);
    //    char block_right = level->getBlock(foot_row, col_right);


    //    bool blocked_below = (   (block_left == 'g' || block_left == 's' || block_left == 'b') || (block_mid == 'g' || block_mid == 's' || block_mid == 'b') || (block_right == 'g' || block_right == 's' || block_right == 'b'));


    //    if (blocked_below && velocity_y > 0)     // if falling and neche block aa jata he
    //    {
    //        on_ground = true;
    //        velocity_y = 0;
    //        pos_y = (foot_row * cs) - character_height;  // snap above block
    //    }
    //    else
    //    {
    //        pos_y = next_y;
    //        !blocked_below ? on_ground = false : on_ground;   // fal
    //    }



    //}

    void applyHorizontalCollision(Level* level);
    //void applyHorizontalCollision(Level* level)
    //{

    //    int cs = level->cell_size;

    //    float next_x = pos_x + velocity_x;


    //    next_x < 0 ? next_x = 0 : next_x;

    //    float max_x = (float)(level->width * cs - character_width);         //why are u running
    //    next_x > max_x ? next_x = max_x : next_x;


    //    int top_row = (int)(pos_y + 5) / cs;  // same shit diffrent function
    //    int mid_row = (int)(pos_y + character_height / 2) / cs;
    //    int bot_row = (int)(pos_y + character_height - 5) / cs;


    //    if (velocity_x > 0)  // RIGHT
    //    {
    //        int right_col = (int)(next_x + character_width) / cs;
    //        if (right_col < level->width)
    //        {
    //            char top = level->getBlock(top_row, right_col);
    //            char mid = level->getBlock(mid_row, right_col);
    //            char bottom = level->getBlock(bot_row, right_col);
    //            if ( (top == 'g' || top == 's' || top == 'b' ) || ( mid == 'g' || mid == 's' || mid == 'b' ) ||  ( bottom == 'g' || bottom == 's' || bottom == 'b' ))
    //            {
    //                next_x = (right_col * cs) - character_width - 1;
    //            }
    //        }
    //    }
    //    else if (velocity_x < 0)  // LEFT
    //    {
    //        int left_col = (int)(next_x + character_width) / cs;
    //        if (left_col < level->width)
    //        {
    //            char top = level->getBlock(top_row, left_col);
    //            char mid = level->getBlock(mid_row, left_col);
    //            char bottom = level->getBlock(bot_row, left_col);
    //            if ((top == 'g' || top == 's' || top == 'b') || (mid == 'g' || mid == 's' || mid == 'b') || (bottom == 'g' || bottom == 's' || bottom == 'b'))
    //            {
    //                next_x = (left_col + 1) * cs + 1;
    //            }
    //        }
    //    }

    //pos_x = next_x;     // finally after all dat

    //}


    void takeDamage(float dmg) {

        if (is_invincible) return;
        if (is_dev) return;

        if (damage_state == 0) {
            hp -= 33.0f;
            damage_state = 1;
            damage_timer = 1.0f;
            is_invincible = true;
            invincible_timer = 0.25f;
        }
        else if (damage_state == 1 && damage_timer > 0) {
            hp -= 33.0f;
            damage_state = 2;
            damage_timer = 1.0f;
            is_invincible = true;
            invincible_timer = 0.25f;
        }
        else if (damage_state == 2 && damage_timer > 0) {
            hp = 0.0f;
            lives--;
            damage_state = 0;
            damage_timer = 0.0f;
            if (lives <= 0) is_alive = false;
            else { is_invincible = true; invincible_timer = 2.0f; }
        }

    }//dmg func end
     

    virtual void shoot(Projectile** pool, int& count, int max_proj)
    {
        if (!is_alive)
            return;
        if (!transform_state->allowsShooting()) 
           return;

        if (current_weapon == 0) return;

         Projectile* p = current_weapon->fire(pos_x, pos_y, facing_direction, aim_angle);

        if ( p != 0)
        {
            // Find empty slot in projectile pool
            for (int i = 0; i < max_proj; i++)
            {
                if (pool[i] == 0 || !pool[i]->isActive())
                {
                    if (pool[i] != 0) delete pool[i];
                    pool[i] = p;
                    return;
                }
            }
            // Pool full, discard projectile
            delete p;
        }
        
    }//shoot

    virtual void updateTimers(float dt);

    //void updateTimers(float dt)
    //{
    //    // Damage state timer
    //    if (damage_timer > 0)
    //    {
    //        damage_timer -= dt;
    //        if (damage_timer <= 0)//clamp
    //        {
    //            // Timer expired: reset to normal if not hit again
    //            damage_state = 0;
    //            damage_timer = 0;
    //        }
    //    }

    //    // Character state timer
    //    if (character_state != 0)
    //    {
    //        state_timer -= dt;  //clamp
    //        if (state_timer <= 0)
    //        {
    //            character_state = 0;
    //            state_timer = 0;
    //        }
    //    }

    //    // Special  timer
    //    if (special_active)
    //    {
    //        special_timer -= dt;
    //        if (special_timer <= 0)//clamp
    //        {
    //            special_active = false;
    //            special_timer = 0;

    //            Fio* fio = dynamic_cast<Fio*>(this);
    //            if (fio && fio->supercharge_active) {
    //                fio->deactivateSupercharge();
    //            }

    //            Marco* marco = dynamic_cast<Marco*>(this);
    //            if (marco) marco->dual_fire_active = false;

    //            Eri* eri = dynamic_cast<Eri*>(this);
    //            if (eri) eri->double_grenade_active = false;


    //        }
    //    }

    //    // its vincing time
    //    if (is_invincible)
    //    {
    //        invincible_timer -= dt;
    //        if (invincible_timer <= 0)//clamp
    //        {
    //            is_invincible = false;
    //            invincible_timer = 0;
    //        }
    //    }

    //    // Melee cooldown
    //     
    //     if (melee_timer > 0)
    //    {
    //         melee_timer -= dt;
    //        if (melee_timer <= 0)//clamp
    //        {
    //            melee_timer = 0;
    //        }
    //    }

    //    // Weapon fire timer
    //    if (current_weapon)
    //        current_weapon->updateTimer(dt);

    //    // fat (fit) check
    //    if (saturation >= 10 && !is_fat)
    //        is_fat = true;

    //    // Anime timer
    //    animation_timer += dt;
    //    if (animation_timer >= 0.1f)
    //    {
    //        animation_timer = 0;
    //        current_frame++;

    //        if (current_frame >= total_frames)
    //            current_frame = 0;
    //    }
    //}





    void spawnGrenade(Projectile** pool, int& count, int max_proj, float start_x, float start_y, float vx, float vy, sf::Texture& grenade_tex);

    //void spawnGrenade(Projectile** pool, int& count, int max_proj, float start_x, float start_y, float vx, float vy, sf::Texture& grenade_tex) {
    //    Projectile* g = new Projectile(start_x, start_y, vx, vy, 20.0f, 16, 16);

    //    g->is_explosive = true;
    //    g->is_grenade = true;
    //    g->blast_radius = 3.0f;

    //    g->sprite.setTexture(grenade_tex);   // txture


    //    for (int i = 0; i < max_proj; i++)
    //    {
    //        if (pool[i] == 0 || !pool[i]->is_active)
    //        {
    //            if (pool[i] != 0) delete pool[i];
    //            pool[i] = g;
    //            return;
    //        }
    //    }
    //    delete g;
    //}
    void throwGrenade(Projectile** pool, int& count, int max_proj, sf::Texture& grenade_tex);

    //void throwGrenade(Projectile** pool, int& count, int max_proj, sf::Texture& grenade_tex)
    //{
    //    if (!is_alive) return;

    //    if (grenade_count <= 0) return;

    //    grenade_count--;


    //    // ballistic arc     aayy  parabola   


    //    // Initial velocity: forward + upward
    //    float vx = (facing_direction == 1) ? 5.0f : -5.0f;
    //    float vy = -8.0f;



    //    float start_x = pos_x + (facing_direction == 1 ? character_width + 10 : -10);
    //    float start_y = pos_y + (character_height / 2);

    //    spawnGrenade(pool,count, max_proj,start_x, start_y,vx,vy,grenade_tex);
    //    Eri* eri = dynamic_cast<Eri*>(this);
    //    if (eri && eri->double_grenade_active) {
    //        float extra_vx = (facing_direction == 1) ? 2.5f : -2.5f;
    //        spawnGrenade(pool, count, max_proj, start_x, start_y, vx+extra_vx, vy, grenade_tex);

    //    }



    //}//yeetus
    bool melee(Enemy** enemies, int max_enemies, Scoring* game_score, bool player_airborne);
    void render(sf::RenderWindow& window, float cam_x, float cam_y)
    {

        if (!is_alive) return;
        // Blink during invinc
        if (is_invincible)
        {
            //  skip  every other frame
            int tick = (int)(invincible_timer * 10);
            if (tick % 2 == 0) return;
        }



        float draw_x = pos_x - cam_x;   // camera offset
        float draw_y = pos_y - cam_y  ;

        sprite.setTextureRect( sf::IntRect ( current_frame * frame_width, 0, frame_width, frame_height) );   // fkn set rectangle to allow karte na bahi sare tutorialsmein to he
    

        if (facing_direction == 0)   // LEFT
        {
            sprite.setScale(-2.0f, 2.0f);
            
            sprite.setPosition(draw_x + character_width, draw_y);  // ++ width bcs yeh
        }
        else
        {
        
            sprite.setScale(2.0f, 2.0f);

            sprite.setPosition(draw_x, draw_y);
        
        }

        window.draw(sprite);
    
    }//rndr end

};
class Marco : public Character
{
    // new shi in marc polo
protected:
    bool dual_fire_active;
	float dual_timer;
    bool burst_pending;
    float burst_timer;
    int burst_dir;
    Projectile** burst_pool;
    int* burst_count_ref;
    int burst_max;
    sf::Texture mummy_tex, zombie_tex; // Animetion
    int prev_state;               
    public:
        void setDualfireactive(bool condition) {
            dual_fire_active = condition;
            if (!condition) {                          
                burst_pending = false;
            }
     }
    Marco() {
		dual_fire_active = false;
		dual_timer = 0.0f;

		grenade_count = 8;   // 2 less grnds
        old_grenade_count = grenade_count;

        texture.loadFromFile("Assets/marco.png");
        sprite.setTexture(texture);
        sprite.setScale(2.0f, 2.0f);

        //Burst Fire
        burst_pending = false;
        burst_timer = 0.0f;
        burst_dir = 0;
        burst_pool = nullptr;
        burst_count_ref = nullptr;
        burst_max = 0;


        //animation

        frame_width = 80;
        frame_height = 56;
        total_frames = 38;
        prev_state = 0;                                          
        mummy_tex.loadFromFile("Assets/marco_mummy.png");           
        zombie_tex.loadFromFile("Assets/marco_zombie.png");


    }
    void onWeaponAssigned()  {
        if (current_weapon) {
            current_weapon->setFireCooldown(current_weapon->getFireCooldown() * 0.8f);
        }
    }

    void activateSpecial()
    {
        dual_fire_active = true;

        special_active = true;

        dual_timer  = 10.0f;  

        special_timer = 10.0f;
    }

    void shoot(Projectile** pool, int& count, int max_proj) override {
        bool will_fire = current_weapon && current_weapon->isReadyToFire() && is_alive && transform_state->allowsShooting();

      Character::shoot(pool, count, max_proj);
        if (dual_fire_active && will_fire && !burst_pending) {
            burst_pending = true;
            burst_timer = 0.15f;          
            burst_dir = (facing_direction == 1) ? 0 : 1;   
            burst_pool = pool;
            burst_count_ref = &count;
            burst_max = max_proj;
        }
    }
    void updateTimers(float dt) override {
        syncTransformState(); 

        Character::updateTimers(dt);
        if (dual_fire_active) {
            dual_timer -= dt;
            if (dual_timer <= 0.0f) {
                dual_fire_active = false;          
                dual_timer = 0.0f;
                burst_pending = false;           
            }
        }
        if (burst_pending) {
            burst_timer -= dt;
            if (burst_timer <= 0.0f) {
                burst_pending = false;

               
                int saved_dir = facing_direction;
                facing_direction = burst_dir;       

                if (current_weapon) current_weapon->resetFireTimer(); 
                Character::shoot(burst_pool, *burst_count_ref, burst_max);

                facing_direction = saved_dir;        
            }
       }

    }
    virtual bool canPierceShield() const override {  // new anime tion fkr on da last day 
        return true;
    }

    void syncTransformState() {
        if (character_state == prev_state) return;
        prev_state = character_state;
        current_frame = 0; animation_timer = 0.0f;
        if (character_state == 2) {          // mummy
            sprite.setTexture(mummy_tex);
            frame_width = 60; frame_height = 75;
        }
        else if (character_state == 1) {   // zombie
            sprite.setTexture(zombie_tex);
            frame_width = 60; frame_height = 60;
        }
        else {                             // normal
            sprite.setTexture(texture);
            frame_width = 80;    frame_height = 56;
        }
    }

    void playIdleAnim(float dt) override {
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 0, 6, 4.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 0, 12, 6.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
    }
    void moveLeft(float dt) override {
        velocity_x = -speed * transform_state->getSpeedMultiplier();
        facing_direction = 0;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 6, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    void moveRight(float dt) override {
        velocity_x = speed * transform_state->getSpeedMultiplier();
        facing_direction = 1;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 6, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    void jump(float dt) override {
        if (on_ground) jump_count = 0;
        if (jump_buffer <= 0 && on_ground) {
            velocity_y = -13.0f; on_ground = false;
            jump_count = 1; jump_buffer = double_jump_cooldown;
        }
        else if (!on_ground && jump_count == 1 && jump_buffer <= 0) {
            velocity_y = -10.0f; jump_count = 2; jump_buffer = double_jump_cooldown;
        }
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 18, 6, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 36, 7, 8.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);
    }

    };



class Tarma: public Character {
protected:
    bool immunity_active;
    float immunity_timer;
    sf::Texture mummy_tex, zombie_tex;  
    int prev_char_state;                
public:
    Tarma() {
       immunity_active = false;
        immunity_timer = 0.0f;

        old_grenade_count = grenade_count;
        speed = 4.0f * 0.8f;   
        hp = 80.0f;                

        texture.loadFromFile("Assets/tarma.png");
        sprite.setTexture(texture);
        sprite.setScale(2.0f, 2.0f);

        frame_width = 36;
        frame_height = 50;
        total_frames = 22;

        prev_char_state = 0;                                           
        mummy_tex.loadFromFile("Assets/tarma_mummy.png");             
        zombie_tex.loadFromFile("Assets/tarma_zombie.png");

    }


    void activateSpecial() override
    {
// no dmg 20 secs
        immunity_active = true;
        
        immunity_timer = 20.0f;
        
        is_invincible = true;
        
        invincible_timer = 20.0f;


        special_active = true;
        
        special_timer = 20.0f;

    }


    void syncTransformState() {
        if (character_state == prev_char_state) return;
        prev_char_state = character_state;
        current_frame = 0; animation_timer = 0.0f;
        if (character_state == 2) {
            sprite.setTexture(mummy_tex);
            frame_width = 50; frame_height = 75;
        }
        else if (character_state == 1) {
            sprite.setTexture(zombie_tex);
            frame_width = 60; frame_height = 60;
        }
        else {
            sprite.setTexture(texture);
            frame_width = 36;  
            frame_height = 50;  
        }
    }


    void updateTimers(float dt) override {
        syncTransformState();
        Character::updateTimers(dt);
    }

    void playIdleAnim(float dt) override {
            if (character_state == 2)
                animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
            else if (character_state == 1)
                animateSprite(sprite, animation_timer, current_frame, 0, 12, 6.0f, frame_width, frame_height, dt);
            else
                animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
        }

    void moveLeft(float dt) override {
        velocity_x = -speed * transform_state->getSpeedMultiplier();
        facing_direction = 0;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    void moveRight(float dt) override {
        velocity_x = speed * transform_state->getSpeedMultiplier();
        facing_direction = 1;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    void jump(float dt) override {
        if (on_ground) jump_count = 0;
        if (jump_buffer <= 0 && on_ground) {
            velocity_y = -13.0f; on_ground = false;
            jump_count = 1;
            jump_buffer = double_jump_cooldown;
        }
        else if (!on_ground && jump_count == 1 && jump_buffer <= 0) {
            velocity_y = -10.0f; jump_count = 2; jump_buffer = double_jump_cooldown;
        }
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 36, 7, 8.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);
    }

};
class Eri : public Character {
protected:
    bool double_grenade_active;
    float double_grenade_timer;
    sf::Texture mummy_tex, zombie_tex; // ADDED
    int prev_char_state;               // ADDED
    public:
        void setDoublegrenadeactive(bool condition) {
            double_grenade_active = condition;
      }
        bool getDoublegrenadeActive() {
            return double_grenade_active;
      }
    Eri() {
        double_grenade_active = false;
        double_grenade_timer = 0.0f;

        grenade_count = 20; 
        old_grenade_count = grenade_count;
        melee_cooldown = 999999999.0f; // no melee , i anit makin anthr var to stop melee
        grenade_blast_radius = 4.5f;

        texture.loadFromFile("Assets/eri.png");
        sprite.setTexture(texture);
        sprite.setScale(2.0f, 2.0f);

        frame_width = 38;
        frame_height = 57;
        total_frames = 22;
        prev_char_state = 0;                                        // ADDED
        mummy_tex.loadFromFile("Assets/eri_mummy.png");            // ADDED
        zombie_tex.loadFromFile("Assets/eri_zombie.png");          // ADDED
    }
    void onWeaponAssigned() {
        if (current_weapon) {
            current_weapon->setFireCooldown(current_weapon->getFireCooldown() * 1.25f);
        }
    }

    void activateSpecial() override
    {
        double_grenade_active = true;
        double_grenade_timer = 10.0f;

        special_active = true;
        special_timer = 10.0f;
    }
    void updateTimers(float dt)override {
        syncTransformState();
        Character::updateTimers(dt);
        if (double_grenade_active) {
            double_grenade_timer -= dt;
            if (double_grenade_timer <= 0.0f) {
                double_grenade_active = false;
                double_grenade_timer = 0.0f;
            }
        }
    }

        // ADDED
        void playIdleAnim(float dt) override {
            if (character_state == 2)
                animateSprite(sprite, animation_timer, current_frame, 0, 5, 4.0f, frame_width, frame_height, dt);
            else if (character_state == 1)
                animateSprite(sprite, animation_timer, current_frame, 0, 12, 6.0f, frame_width, frame_height, dt);
            else
                animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
        }

        // ADDED
        void moveLeft(float dt) override {
            velocity_x = -speed * transform_state->getSpeedMultiplier();
            facing_direction = 0;
            if (character_state == 2)
                animateSprite(sprite, animation_timer, current_frame, 5, 12, 8.0f, frame_width, frame_height, dt);
            else if (character_state == 1)
                animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
            else
                animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        }

        // ADDED
        void moveRight(float dt) override {
            velocity_x = speed * transform_state->getSpeedMultiplier();
            facing_direction = 1;
            if (character_state == 2)
                animateSprite(sprite, animation_timer, current_frame, 5, 12, 8.0f, frame_width, frame_height, dt);
            else if (character_state == 1)
                animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
            else
                animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        }

        // ADDED
        void jump(float dt) override {
            if (on_ground) jump_count = 0;
            if (jump_buffer <= 0 && on_ground) {
                velocity_y = -13.0f; on_ground = false;
                jump_count = 1; jump_buffer = double_jump_cooldown;
            }
            else if (!on_ground && jump_count == 1 && jump_buffer <= 0) {
                velocity_y = -10.0f; jump_count = 2; jump_buffer = double_jump_cooldown;
            }
            if (character_state == 2)
                animateSprite(sprite, animation_timer, current_frame, 17, 6, 8.0f, frame_width, frame_height, dt);
            else if (character_state == 1)
                animateSprite(sprite, animation_timer, current_frame, 36, 7, 8.0f, frame_width, frame_height, dt);
            else
                animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);
        }

    
    void syncTransformState() {
        if (character_state == prev_char_state) return;
        prev_char_state = character_state;
        current_frame = 0; animation_timer = 0.0f;
        if (character_state == 2) {
            sprite.setTexture(mummy_tex);
            frame_width = 50; frame_height = 60;
        }
        else if (character_state == 1) {
            sprite.setTexture(zombie_tex);
            frame_width = 60; frame_height = 60;
        }
        else {
            sprite.setTexture(texture);
            frame_width = 38; frame_height = 57;
        }
    }



};



class Fio : public Character {
protected:
    bool supercharge_active;
    float supercharge_timer;
    float base_fire_cooldown;
    sf::Texture mummy_tex, zombie_tex; // 
    int prev_char_state;               // 
    public:
        bool getSuperchargective() {
        return supercharge_active;
    }
        float getSuperchargeTimer() {
        return supercharge_timer;
       }
        float getBaseFireCooldown() {
            return base_fire_cooldown;
       }
    Fio(){
        supercharge_active = false;
        supercharge_timer = 0.0f;
        base_fire_cooldown = 0.0f;

        grenade_count = 8;
        old_grenade_count = grenade_count;

        texture.loadFromFile("Assets/fio.png");
        sprite.setTexture(texture);
        sprite.setScale(2.0f, 2.0f);

        frame_width = 40;
        frame_height = 56;
        total_frames = 22;
        prev_char_state = 0;                                         
        mummy_tex.loadFromFile("Assets/fio_mummy.png");             
        zombie_tex.loadFromFile("Assets/fio_zombie.png");          
    }
    void onWeaponAssigned() {
        if (current_weapon) {
            current_weapon->setFireCooldown(current_weapon->getFireCooldown() * 0.909f);
            base_fire_cooldown = current_weapon->getFireCooldown();
        }
    }
    void activateSpecial() override
    {
        if (!current_weapon) return;
        if (!supercharge_active) {        
            base_fire_cooldown = current_weapon->getFireCooldown();                        
            current_weapon->setFireCooldown(base_fire_cooldown * 0.5f);
        }
        supercharge_active = true;
        supercharge_timer = 10.0f;
       
        special_active = true;
        special_timer = 10.0f;
    }
    void deactivateSupercharge() {
        if (current_weapon && base_fire_cooldown > 0.0f) {
            current_weapon->setFireCooldown(base_fire_cooldown);
        }
        supercharge_active = false;
    }
    void updateTimers(float dt) override {
        Character::updateTimers(dt);
        syncTransformState();
        if (supercharge_active) {
            supercharge_timer -= dt;
            if (supercharge_timer <= 0.0f) {
                supercharge_timer = 0.0f;
                if (supercharge_active)
                    deactivateSupercharge();
            }
        }
    }
    void syncTransformState() {
        if (character_state == prev_char_state) return;
        prev_char_state = character_state;
        current_frame = 0; animation_timer = 0.0f;
        if (character_state == 2) {
            sprite.setTexture(mummy_tex);
            frame_width = 55; frame_height = 60;
        }
        else if (character_state == 1) {
            sprite.setTexture(zombie_tex);
            frame_width = 60; frame_height = 60;
        }
        else {
            sprite.setTexture(texture);
            frame_width = 40;  
            frame_height = 56;  
        }
    }


    // ADDED
    void playIdleAnim(float dt) override {
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 0, 12, 6.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 0, 4, 4.0f, frame_width, frame_height, dt);
    }

    // ADDED
    void moveLeft(float dt) override {
        velocity_x = -speed * transform_state->getSpeedMultiplier();
        facing_direction = 0;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    // ADDED
    void moveRight(float dt) override {
        velocity_x = speed * transform_state->getSpeedMultiplier();
        facing_direction = 1;
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 12, 24, 10.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 4, 12, 8.0f, frame_width, frame_height, dt);
    }

    // ADDED
    void jump(float dt) override {
        if (on_ground) jump_count = 0;
        if (jump_buffer <= 0 && on_ground) {
            velocity_y = -13.0f; on_ground = false;
            jump_count = 1; jump_buffer = double_jump_cooldown;
        }
        else if (!on_ground && jump_count == 1 && jump_buffer <= 0) {
            velocity_y = -10.0f; jump_count = 2; jump_buffer = double_jump_cooldown;
        }
        if (character_state == 2)
            animateSprite(sprite, animation_timer, current_frame, 16, 5, 8.0f, frame_width, frame_height, dt);
        else if (character_state == 1)
            animateSprite(sprite, animation_timer, current_frame, 36, 7, 8.0f, frame_width, frame_height, dt);
        else
            animateSprite(sprite, animation_timer, current_frame, 16, 6, 8.0f, frame_width, frame_height, dt);
    }





};


class FusionCompanion : public Character {
protected:
    float ai_fire_timer;
    float ai_melee_timer;
    float ai_grenade_timer;
    float active_timer;
    float duration;
    bool is_active_companion;
    public:
        bool isActiveCompanion() const { 
           return is_active_companion;
           }
        void resetCompanion() {
            is_active_companion = false;
            is_alive = false;
        }
        void activate(float px, float py, Character** chars, int num_chars);
        void updateAI(Character* player, Level* level, Enemy** enemies, int max_enemies, Projectile** pool, int& proj_count, int max_proj, Scoring* score, float dt);
    FusionCompanion()  {
        duration = 30.0f;       
        is_active_companion = false;

        lives = 9999;           // immortal   ( where is OMNI MANNNN    WHERE IS HE EEE E        gets destroyed ) sigma from sarim

        texture.loadFromFile("Assets/fusion.png");
        sprite.setTexture(texture);
        sprite.setScale(2.0f, 2.0f);

        grenade_tex.loadFromFile("Assets/grenade_tex.png");
        grenade_sprite.setTexture(grenade_tex);
        frame_width = 50;
        frame_height = 50;
        total_frames = 5;
    }  
    
    void activateSpecial() override {
    
    }
    
};

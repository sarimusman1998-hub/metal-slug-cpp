#include "Vehicle.h"
#include "Character.h"
#include "Level.h"



Vehicle::Vehicle(float x, float y, float w, float h, float spd, float hp_val)
{
    pos_x = x;
    pos_y = y;

    velocity_x = 0.0f;
    velocity_y = 0.0f;

    hp = hp_val;
    max_hp = hp_val;

    speed = spd;

    width = (int)w;
    height = (int)h;

    is_occupied = false;
    is_alive = true;
    on_ground = false;

    driver = nullptr;

    fire_timer = 0.0f;
    fire_cooldown = 0.1f;

    facing_dir = 1;

    facing_dir = 1;
    current_frame = 0;     
    frame_width = 96;
    frame_height = 160;
    anim_timer = 0.0f;  
}



Vehicle::~Vehicle() {}




float Vehicle::getPosX()      const 
{ 
    return pos_x; 
}
float Vehicle::getPosY()      const
{ 
    return pos_y; 
}
int   Vehicle::getWidth()     const
{
    return width;
}
int   Vehicle::getHeight()    const 
{
    return height;
}
float Vehicle::getHP()        const
{
    return hp; 
}
float Vehicle::getMaxHP()     const
{
    return max_hp;
}
bool  Vehicle::isAlive()      const
{
    return is_alive;
}
bool  Vehicle::isOccupied()   const 
{
    return is_occupied;
}
int   Vehicle::getFacingDir() const
{
    return facing_dir;
}

void Vehicle::moveLeft() { velocity_x = -speed; facing_dir = 0; }
void Vehicle::moveRight() { velocity_x = speed; facing_dir = 1; }
void Vehicle::stopHorizontal() { velocity_x = 0.0f; }

void Vehicle::jump() {
    if (on_ground) {
        velocity_y = -15.0f;
        on_ground = false;
        jump_count = 1;
    }
    else if (jump_count == 1) {
        velocity_y = -15.0f;
        jump_count = 2;
    }
}

void Vehicle::updateTimers(float dt) {
    if (fire_timer > 0.0f)
      fire_timer -= dt;
}

void Vehicle::Driver() {
    if (driver) {
        driver->setPosition(pos_x + (float)width * 0.5f - 30.0f,
            pos_y + 5.0f);
    }
}

bool Vehicle::isNearPlayer(Character* player, float range) {
    float dx = player->getPosX() - pos_x;
    float dy = player->getPosY() - pos_y;
    return (dx * dx + dy * dy) < range * range;   //radiius
}
void Vehicle::enter(Character* p) {
    driver = p;
    is_occupied = true;
    p->setInVehicle(true);
    Tarma* t = dynamic_cast<Tarma*>(p);

    if (t) {
        fire_cooldown *= 0.75f;

        float extra = max_hp * 0.2f;

        max_hp += extra;
        hp += extra;
    }
}
void Vehicle::exit() {
    if (driver) {
          driver->setInVehicle(false);
        driver->setPosition(pos_x + (float)width * 0.5f - 30.0f, pos_y - 40.0f);
        driver->resetMovement();
        is_occupied = false;
        Tarma* t = dynamic_cast<Tarma*>(driver);

        if (t) {
            fire_cooldown /= 0.75f;

            float bonus = max_hp / 1.2f * 0.2f;

            max_hp -= bonus;

            if (hp > max_hp) {
                hp = max_hp;
            }
        }

        driver = nullptr;
    }

    is_occupied = false;
}
void Vehicle::takeDamage(float dmg) {
    hp -= dmg;

    if (hp > 0.0f) {
        return;
    }

    hp = 0.0f;
    is_alive = false;

    if (driver) {
        driver->setPosition(pos_x + (float)width * 0.5f - 30.0f, pos_y - 60.0f);
        driver->resetMovement();

        Tarma* t = dynamic_cast<Tarma*>(driver);

        if (!t) {
            driver->takeDamage(1.0f);
        }

        driver = nullptr;
        is_occupied = false;
    }
}
void Vehicle::render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive) {
        return;
    }
    float sx, sy;
    if (frame_width > 0) {                                    
        sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height * 1.5f));
        sx = (float)width / (float)frame_width;   
        sy = (float)height / (float)frame_height;  
    }
    else {                                                       
        sx = (float)width / (float)texture.getSize().x;
        sy = (float)height / (float)texture.getSize().y;
    }
    if (facing_dir == 0) {
        sprite.setScale(-2.5f, 2.5f);
        sprite.setPosition(pos_x - cam_x + (float)width, pos_y - cam_y-frame_height * 1.5f);
    }
    else {
        sprite.setScale(2.5f, 2.5f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y - frame_height * 1.5f);
    }

    window.draw(sprite);
}
void Vehicle::applyGravity(Level* level) {
    const float gravity = 0.5f;
    const float terminal = 10.0f;

    int cs = level->getCellSize();

    velocity_y += gravity;

    if (velocity_y > terminal) {
        velocity_y = terminal;
    }

    float next_y = pos_y + velocity_y;

    int foot = (int)(next_y + (float)height) / cs;
    int col = (int)(pos_x + (float)width * 0.5f) / cs;

    col = (col < 0) ? 0 : (col >= level->getWidth() ? level->getWidth() - 1 : col);

    if (foot < 0 || foot >= level->getHeight()) {
        on_ground = true;
        velocity_y = 0.0f;
        return;
    }

    char b = level->getBlock(foot, col);

    if ((b == 'g' || b == 's' || b == 'b') && velocity_y > 0.0f) {
        on_ground = true;
        velocity_y = 0.0f;
        pos_y = (float)(foot * cs) - (float)height;
        jump_count = 0;
    }
    else {
        pos_y = next_y;
        on_ground = false;
    }
}
void Vehicle::applyHorizontalCollision(Level* level) {  //same mother fk shit i did last week aaaaaaaaaaAAAAAAAAAAaAaAaaAaaAaA
    int cellSize = level->getCellSize();

    float next_x = pos_x + velocity_x;

    if (next_x < 0.0f) 
        next_x = 0.0f;

    float max_x = (float)(level->getWidth() * cellSize - width);

    if (next_x > max_x) next_x = max_x;

    int mid_row = (int)(pos_y + (float)height * 0.5f) / cellSize;

    int top_row = (int)(pos_y + 4.0f) / cellSize;

    int bot_row = (int)(pos_y + (float)height - 4.0f) / cellSize;

    if (top_row < 0)
    {
        top_row = 0;
    }
    if ( bot_row >= level->getHeight() ) 
    { 
        bot_row = level->getHeight() - 1;
    }
    if (mid_row < 0)
    {
        mid_row = 0;
    }
    if (mid_row >= level->getHeight()) 
    { 
        mid_row = level->getHeight() - 1;
    }

    if (velocity_x > 0.0f) {

        int rc = (int)(next_x + (float)width) / cellSize;

        if (rc < level->getWidth())
        {
            char t = level->getBlock(top_row, rc);

            char m = level->getBlock(mid_row, rc);

            char b_b = level->getBlock(bot_row, rc);
            if ((t == 'g' || t == 's' || t == 'b') || (m == 'g' || m == 's' || m == 'b') ||    (b_b == 'g' || b_b == 's' || b_b == 'b'))
            {
                next_x = (float)(rc * cellSize) - (float)width - 1.0f;

                velocity_x = 0.0f;
            }
        }
    }

    else if (velocity_x < 0.0f) {

        int lc = (int)(next_x) / cellSize;

        if (lc >= 0 && lc < level->getWidth())
        {
            char t = level->getBlock(top_row, lc);
           
            char m = level->getBlock(mid_row, lc);
            
            char b = level->getBlock(bot_row, lc);
            if ((t == 'g' || t == 's' || t == 'b') ||  (m == 'g' || m == 's' || m == 'b') ||   (b == 'g' || b == 's' || b == 'b'))
            {
                next_x = (float)((lc + 1) * cellSize) + 1.0f;
                velocity_x = 0.0f;
            }
        }
    }
    pos_x = next_x;
}
//Metal Slug
MetalSlug::MetalSlug(float x, float y) : Vehicle(x, y, 128.0f, 72.0f, 5.0f, 100.0f) {
    fire_cooldown = 0.08f;
    texture.loadFromFile("Assets/metal_slug.png");
    sprite.setTexture(texture);
    sprite.setScale(4.0f, 2.0f);

    bullet_tex.loadFromFile("Assets/bullet.png");
    frame_width = 65;  
    frame_height = 65;   
    fire_flash_timer = 0.0f; 
}

void MetalSlug::updateTimers(float dt) {
    Vehicle::updateTimers(dt);  

    if (fire_flash_timer > 0.0f) {   
        fire_flash_timer -= dt;
        if (fire_flash_timer < 0.0f) fire_flash_timer = 0.0f;
    }

    int start, count; float fps;
    if (fire_flash_timer > 0.0f) {
        start = 23; count = 10; fps = 12.0f;  // SHOOT
    }
    else if (velocity_x != 0.0f) {
        start = 3; count = 20; fps = 12.0f;  // DRIVE
    }
    else {
        start = 0; count = 3; fps = 6.0f;  // IDLE
    }

    if (current_frame < start || current_frame >= start + count) {
        current_frame = start; anim_timer = 0.0f;
    }
    anim_timer += dt;
    if (anim_timer >= 1.0f / fps) {
        anim_timer = 0.0f;
        if (++current_frame >= start + count) current_frame = start;
    }
}

Projectile* MetalSlug::fire() {
    if (fire_timer > 0.0f) 
      return nullptr;
    fire_flash_timer = 0.3f;            // ADDED

    fire_timer = fire_cooldown;
    float bx = (facing_dir == 1) ? pos_x + (float)width + 5.0f : pos_x - 20.0f;
    float by = pos_y + (float)height * 0.4f;
    float vx = (facing_dir == 1) ? 14.0f : -14.0f;
    Projectile* p = new Projectile(bx, by, vx, 0.0f, 3.0f, 16, 8);
    p->setSpriteTexture(bullet_tex);
    p->sprite.setScale(3.5f, 3.5f);
    return p;
}



//Slug Flyer
SlugFlyer::SlugFlyer(float x, float y): Vehicle(x, y, 96.0f, 56.0f, 4.5f, 80.0f) {

    fire_cooldown = 0.16f;
    missiles = 4;

    texture.loadFromFile("Assets/slug_flyer.png");
    sprite.setTexture(texture);

    bullet_tex.loadFromFile("Assets/bullet.png");
    missile_tex.loadFromFile("Assets/rocket.png");
    frame_width = 130;    
    frame_height = 130;   
    fire_flash_timer = 0.0f;  
}
void SlugFlyer::applyGravity(Level* level) {
    int cs = level->getCellSize();
    velocity_y += 0.08f;
    if (velocity_y > 4.0f) velocity_y = 4.0f;
    if (velocity_y < -4.0f) velocity_y = -4.0f;
    pos_y += velocity_y;

    float min_y = 3.0f * (float)cs;
    float max_y = (float)((level->getHeight() - 2) * cs - height);
    if (pos_y < min_y) {
        pos_y = min_y;
        velocity_y = 0.0f;
    }
    if (pos_y > max_y) {
        pos_y = max_y;
        velocity_y = 0.0f;
    }
}
Projectile* SlugFlyer::fire() {
    if (fire_timer > 0.0f) {
        return nullptr;
    }

    fire_timer = fire_cooldown;
    fire_flash_timer = 0.25f;         


    float bx = (facing_dir == 1) ? pos_x + (float)width + 5.0f : pos_x - 20.0f;
    float by = pos_y + (float)height * 0.5f;

    float vx = (facing_dir == 1) ? 7.0f : -7.0f;

    Projectile* p = new Projectile(bx, by, vx, 0.0f, 3.0f, 16, 8);

    p->setSpriteTexture(bullet_tex);
    p->sprite.setScale(3.5f, 3.5f);  
    return p;
}

void SlugFlyer::updateTimers(float dt) {
    Vehicle::updateTimers(dt);

    if (fire_flash_timer > 0.0f) {
        fire_flash_timer -= dt;
        if (fire_flash_timer < 0.0f) fire_flash_timer = 0.0f;
    }

    int start, count; float fps = 8.0f;
    if (fire_flash_timer > 0.0f) {
        start = 5; count = 2;  //  fire
    }
    else if (velocity_x < -0.5f) {
        start = 1; count = 2;  // left
    }
    else if (velocity_x > 0.5f) {
        start = 3; count = 2;  // right
    }
    else {
        start = 0; count = 1;  // 
    }

    if (current_frame < start || current_frame >= start + count) {
        current_frame = start; anim_timer = 0.0f;
    }
    if (count > 1) {
        anim_timer += dt;
        if (anim_timer >= 1.0f / fps) {
            anim_timer = 0.0f;
            if (++current_frame >= start + count) current_frame = start;
        }
    }
}
void SlugFlyer::render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive) 
        return;
    sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));
    
    float sx = (float)width / (float)frame_width;
    float sy = (float)height / (float)frame_height;
    
    sprite.setScale(sx, sy);                     
    sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    
    window.draw(sprite);
}



Projectile* SlugFlyer::fireMissile() {
    if (missiles <= 0) {
        return nullptr;
    }

    missiles--;

    Projectile* p = new Projectile(
        pos_x + (float)width * 0.5f,
        pos_y + (float)height,
        0.0f,
        7.0f,
        10.0f,
        24,
        16
    );

    p->setExplosive(true, 3.0f);
    p->setGrenade(true);
    p->setSpriteTexture(missile_tex);

    return p;
}
//Slug Mariner
SlugMariner::SlugMariner(float x, float y)
    : Vehicle(x, y, 96.0f, 60.0f, 4.0f, 80.0f) {

    fire_cooldown = 0.12f;

    missiles_h = missiles_v = missiles_r = 3;
    missile_mode = 0;

    texture.loadFromFile("Assets/slug_mariner.png");
    sprite.setTexture(texture);

    bullet_tex.loadFromFile("Assets/bullet.png");
    missile_tex.loadFromFile("Assets/rocket.png");
    frame_width = 62;   
    frame_height = 55;   
}


void SlugMariner::updateTimers(float dt) {
    Vehicle::updateTimers(dt);

    int start, count;
    const float fps = 12.0f;

    if (velocity_y < -0.5f) {       
        start = 0; count = 15;      
    }
    else if (velocity_y > 0.5f) {   
        start = 15; count = 15;     
    }
    else {                           // FIXED: idle = single middle frame 15
        start = 15; count = 1;
    }

    if (current_frame < start || current_frame >= start + count) {
        current_frame = start; anim_timer = 0.0f;
    }
    anim_timer += dt;
    if (anim_timer >= 1.0f / fps) {
        anim_timer = 0.0f;
        if (++current_frame >= start + count) current_frame = start;
    }
}


void SlugMariner::applyGravity(Level* level) {
    int cs = level->getCellSize();

    int cx = (int)(pos_x + (float)width * 0.5f) / cs;
    int cy = (int)(pos_y + (float)height * 0.5f) / cs;

    char c = level->getBlock(cy, cx);

    if (c != 'w') {
        if (driver) {
            driver->setPosition(pos_x + (float)width * 0.5f - 30.0f, pos_y - 40.0f);
            driver->resetMovement();
            driver = nullptr;
        }
        is_occupied = false;
        is_alive = false;  
        return;
    }


    if (c == 'w') {
        velocity_y *= 0.92f;
    }
    else {
        velocity_y += 0.4f;

        if (velocity_y > 5.0f) {
            velocity_y = 5.0f;
        }
    }

    float ny = pos_y + velocity_y;

    float maxY = (float)((level->getHeight() - 1) * cs - height);

    if (ny < 0.0f) {
        ny = 0.0f;
        velocity_y = 0.0f;
    }

    if (ny > maxY) {
        ny = maxY;
        velocity_y = 0.0f;
        on_ground = true;
    }
    else {
        on_ground = false;
    }

    pos_y = ny;
}

Projectile* SlugMariner::fire() {
    if (fire_timer > 0.0f) {
        return nullptr;
    }

    fire_timer = fire_cooldown;

    float bx = (facing_dir == 1) ? pos_x + (float)width + 5.0f : pos_x - 20.0f;
    float by = pos_y + (float)height * 0.5f;

    float vx = (facing_dir == 1) ? 10.0f : -10.0f;

    Projectile* p = new Projectile(bx, by, vx, 0.0f, 3.0f, 16, 8);

    p->setSpriteTexture(bullet_tex);
    p->sprite.setScale(3.5f, 3.5f);  
    return p;
}

Projectile* SlugMariner::fireMissile() {
    if (fire_timer > 0.0f) {
        return nullptr;
    }

    fire_timer = fire_cooldown * 3.0f;

    Projectile* p = nullptr;

    float cx = pos_x + (float)width * 0.5f;
    float cy = pos_y + (float)height * 0.5f;

    if (missile_mode == 0 && missiles_h > 0) {
        missiles_h--;

        float vx = (facing_dir == 1) ? 9.0f : -9.0f;

        p = new Projectile(cx, cy, vx, 0.0f, 5.0f, 24, 12);
    }
    else if (missile_mode == 1 && missiles_v > 0) {
        missiles_v--;

        p = new Projectile(cx, pos_y, 0.0f, -9.0f, 5.0f, 12, 24);
    }
    else if (missile_mode == 2 && missiles_r > 0) {
        missiles_r--;

        float vx = (facing_dir == 1) ? 3.0f : -3.0f;

        p = new Projectile(cx, pos_y, vx, -8.0f, 5.0f, 24, 12);
    }

    if (p) {
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setSpriteTexture(missile_tex);
        p->sprite.setScale(3.5f, 3.5f);
    }

    return p;
}
//Amphibious Slug
AmphibiousSlug::AmphibiousSlug(float x, float y): Vehicle(x, y, 112.0f, 68.0f, 4.5f, 120.0f) {
    fire_cooldown = 0.08f;
    missiles = 4;
    mode = 0;
    ground_tex.loadFromFile("Assets/metal_slug.png");
    fly_tex.loadFromFile("Assets/slug_flyer.png");
    marine_tex.loadFromFile("Assets/slug_mariner.png");
    texture = ground_tex;
    sprite.setTexture(texture);
    bullet_tex.loadFromFile("Assets/bullet.png");
    missile_tex.loadFromFile("Assets/rocket.png");
    frame_width = 65; 
    frame_height = 65;
    fire_flash_timer = 0.0f;

}
void AmphibiousSlug::updateMode(Level* level) {
    int cs = level->getCellSize();

    int cx = (int)(pos_x + (float)width * 0.5f) / cs;
    int cy = (int)(pos_y + (float)height * 0.5f) / cs;

    char c = level->getBlock(cy, cx);

    int new_mode;

    if (c == 'w') {
        new_mode = 2;
    }
    else if (on_ground) {
        new_mode = 0;
    }
    else {
        new_mode = 1;
    }

    if (new_mode == mode) {
 
        return;
    }

    mode = new_mode;
    current_frame = 0;
    anim_timer = 0.0f;
    fire_flash_timer = 0.0f;
    
    switch (mode) {
    case 0:
        sprite.setTexture(ground_tex);
        fire_cooldown = 0.08f;
        frame_width = 65;
        frame_height = 65;
        break;

    case 1:
        sprite.setTexture(fly_tex);
        fire_cooldown = 0.16f;
        frame_width = 130; 
        frame_height = 130;
        break;

    case 2:
        sprite.setTexture(marine_tex);
        fire_cooldown = 0.12f;
        frame_width = 62;
        frame_height = 55;
        break;
    }
}


// ADDED – drive all three modes from one updateTimers
void AmphibiousSlug::updateTimers(float dt) {
    Vehicle::updateTimers(dt);   

    if (fire_flash_timer > 0.0f) {
        fire_flash_timer -= dt;
        if (fire_flash_timer < 0.0f) fire_flash_timer = 0.0f;
    }

    int start, count; float fps;

    if (mode == 0) {
        if (fire_flash_timer > 0.0f) { start = 23; count = 10; fps = 12.0f; }
        else if (velocity_x != 0.0f) { start = 3; count = 20; fps = 12.0f; }
        else { start = 0; count = 3; fps = 6.0f; }
    }
    else if (mode == 1) {
        fps = 8.0f;
        if (fire_flash_timer > 0.0f) { start = 5; count = 2; }
        else if (velocity_x < -0.5f) { start = 1; count = 2; }
        else if (velocity_x > 0.5f) { start = 3; count = 2; }
        else { start = 0; count = 1; }
    }
    else {
        fps = 12.0f;
        if (velocity_y < -0.5f) { start = 0; count = 15; }
        else { start = 15; count = 15; }
    }

    if (current_frame < start || current_frame >= start + count) {
        current_frame = start; anim_timer = 0.0f;
    }
    anim_timer += dt;
    if (anim_timer >= 1.0f / fps) {
        anim_timer = 0.0f;
        if (++current_frame >= start + count) current_frame = start;
    }
}


void AmphibiousSlug::applyGravity(Level* level) {
    int cs = level->getCellSize();

    if (mode == 1) {
        int cs = level->getCellSize();
        velocity_y += 0.08f;
        if (velocity_y > 4.0f) 
           velocity_y = 4.0f;
        if (velocity_y < -4.0f)
          velocity_y = -4.0f;
        float ny = pos_y + velocity_y;
        int foot = (int)(ny + height) / cs;
        int col = (int)(pos_x + width * 0.5f) / cs;
        col = (col < 0) ? 0 : (col >= level->getWidth() ? level->getWidth() - 1 : col);
        if (foot >= 0 && foot < level->getHeight()) {
            char b = level->getBlock(foot, col);
            if ((b == 'g' || b == 's' || b == 'b') && velocity_y > 0.0f) {
                on_ground = true; 
                velocity_y = 0.0f; 
                jump_count = 0;
                pos_y = (float)(foot * cs) - (float)height;
            }
            else {
             pos_y = ny;
             on_ground = false;
             }
        }
        else {
          pos_y = ny; 
          }

        float min_y = 3.0f * (float)cs;
        float max_y = (float)((level->getHeight() - 2) * cs - height);
        if (pos_y < min_y) { 
          pos_y = min_y;
          velocity_y = 0.0f;
          }
        if (pos_y > max_y) { 
           pos_y = max_y;
           velocity_y = 0.0f;
           }
    }
    else if (mode == 2) {
        int ccx = (int)(pos_x + (float)width * 0.5f) / cs;
        int ccy = (int)(pos_y + (float)height * 0.5f) / cs;

        char blk = level->getBlock(ccy, ccx);

        if (blk == 'w') {
            velocity_y *= 0.92f;
        }
        else {
            velocity_y += 0.4f;

            if (velocity_y > 5.0f) {
                velocity_y = 5.0f;
            }
        }

        float ny = pos_y + velocity_y;

        float maxY = (float)((level->getHeight() - 1) * cs - height);

        if (ny < 0.0f) {
            ny = 0.0f;
            velocity_y = 0.0f;
        }

        if (ny > maxY) {
            ny = maxY;
            velocity_y = 0.0f;
            on_ground = true;
        }
        else {
            on_ground = false;
        }

        pos_y = ny;
    }
    else {
        velocity_y += 0.5f;

        if (velocity_y > 10.0f) {
            velocity_y = 10.0f;
        }

        float ny = pos_y + velocity_y;

        int foot = (int)(ny + (float)height) / cs;
        int col = (int)(pos_x + (float)width * 0.5f) / cs;

        col = (col < 0) ? 0 : (col >= level->getWidth() ? level->getWidth() - 1 : col);

        if (foot < 0 || foot >= level->getHeight()) {
            on_ground = true;
            velocity_y = 0.0f;
            return;
        }

        char b = level->getBlock(foot, col);

        if ((b == 'g' || b == 's' || b == 'b') && velocity_y > 0.0f) {
            on_ground = true;
            velocity_y = 0.0f;
            jump_count = 0;

            pos_y = (float)(foot * cs) - (float)height;
        }
        else {
            pos_y = ny;
            on_ground = false;
        }
    }
}
void AmphibiousSlug::moveUp() {
    if (mode == 0) {
        jump();
    }
    else {
        velocity_y = -speed * 0.75f;
    }
}

void AmphibiousSlug::moveDown() {
    if (mode != 0) {
        velocity_y = speed * 0.75f;
    }
}

void AmphibiousSlug::stopVertical() {
    if (mode == 1) {
        velocity_y *= 0.85f;
    }

    if (mode == 2) {
        velocity_y *= 0.9f;
    }
}

void AmphibiousSlug::jump() {
    if (mode != 0) {
        return;
    }

    if (on_ground) {
        velocity_y = -13.0f;
        on_ground = false;
        jump_count = 1;
    }
    else if (jump_count == 1) {
        velocity_y = -10.0f;
        jump_count = 2;
    }
}
Projectile* AmphibiousSlug::fire() {
    if (fire_timer > 0.0f) {
        return nullptr;
    }

    fire_timer = fire_cooldown;

    float spd = (mode == 1) ? 7.0f : 14.0f;

    float bx = (facing_dir == 1) ? pos_x + (float)width + 5.0f : pos_x - 20.0f;
    float by = pos_y + (float)height * 0.4f;

    float vx = (facing_dir == 1) ? spd : -spd;

    Projectile* p = new Projectile(bx, by, vx, 0.0f, 3.0f, 16, 8);

    p->setSpriteTexture(bullet_tex);
    p->sprite.setScale(3.5f, 3.5f);
    return p;
}

Projectile* AmphibiousSlug::fireMissile() {
    if (missiles <= 0) {
        return nullptr;
    }

    missiles--;

    float cx = pos_x + (float)width * 0.5f;

    float dvx = (facing_dir == 1) ? 2.0f : -2.0f;
    float dvy = (mode == 2) ? -8.0f : 7.0f;

    float by = (mode == 2) ? pos_y : pos_y + (float)height;

    Projectile* p = new Projectile(cx, by, dvx, dvy, 10.0f, 24, 16);

    p->setExplosive(true, 3.0f);
    p->setGrenade(true);
    p->setSpriteTexture(missile_tex);
    p->sprite.setScale(3.5f, 3.5f);
    return p;
}

void AmphibiousSlug::render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive) return;

    sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));
    float sx = (float)width / (float)frame_width;
    float sy = (float)height / (float)frame_height;

    if (mode == 1) {
        sprite.setScale(2.5f, 2.5f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    }
    else {
        if (facing_dir == 0) {
            sprite.setScale(-2.5f, 2.5f);
            sprite.setPosition(pos_x - cam_x + (float)width, pos_y - cam_y);
        }
        else {
            sprite.setScale(2.5f, 2.5f);
            sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
        }
    }
    window.draw(sprite);
}

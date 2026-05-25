#include "Boss.h"
#include "Level.h"

using namespace sf;


/*oid addToPool(Projectile* p, Projectile** pool, int& proj_count, int max_proj) {
    for (int i = 0; i < max_proj; i++) {
        if (pool[i] == nullptr || !pool[i]->isActive()) {
            delete pool[i];
            pool[i] = p;
            return;
        }
    }
    delete p;
}*/
void IronNokana::move(Character* player, Level* level, float dt) {
    if (!is_retreating && hp <= 15.0f)
        is_retreating = true;
    if (is_retreating) {
        velocity_x = speed * 4.0f;
        pos_x += velocity_x;
        if (pos_x > (float)(level->getWidth() * level->getCellSize()) + 64.0f) {
            has_retreated = true;
            is_alive = false;
 }
        return;
    }
    float dx = player->getPosX() - pos_x;
    float abs_dx = (dx >= 0.0f) ? dx : -dx;
    float effective_speed = speed * power_boost;
    if (abs_dx > 380.0f) 
      velocity_x = (dx > 0.0f) ? effective_speed : -effective_speed;
    else if (abs_dx < 200.0f) 
      velocity_x = (dx > 0.0f) ? -effective_speed * 0.4f : effective_speed * 0.4f;
    else   
      velocity_x = 0.0f;

    checkAndJump(level);


}
void IronNokana::attack(Character* player, Projectile** pool, int& proj_count,int max_proj, sf::Texture& bullet_tex, float dt) {

    if (is_retreating)
      return;
    float dx = player->getPosX() - pos_x;
    float dy = player->getPosY() - pos_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f)
       dist = 1.0f;

       //Missiles
    missile_timer += dt;
    if (missile_timer >= 2.5f && dist < 900.0f) {
        missile_timer = 0.0f;

        float vx = (dx > 0.0f) ? 5.0f : -5.0f;
        Projectile* p = new Projectile( pos_x + (float)enemy_width * 0.5f, pos_y, vx, -10.0f,  5.0f, 24, 12
        );
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setSpriteTexture(missile_tex_boss);
        addToPool(p, pool,proj_count, max_proj);
    }
    //Flames
    flame_timer += dt;
    if (flame_timer >= 3.0f && dist < 350.0f) {
        flame_timer = 0.0f;
           
           float flame_x = (dx > 0.0f) ? 6.0f : -6.0f;
           float sx = pos_x + (dx > 0.0f ? (float)enemy_width : 0.0f);
           float sy = pos_y + (float)enemy_height - 15.0f;
           for (int i = 0; i < 6; i++) {
               Projectile* p = new Projectile( sx + flame_x * (float)i * 0.8f, sy, flame_x, 0.0f, 2.0f / 30.0f, 32,1);
               p->setFromEnemy(true);
               p->setisFire(true);
               p->setLifespan(1.2f);
               p->setSpriteTexture(flame_tex_boss);
               addToPool(p, pool, proj_count, max_proj);

           }

    }
         //Fire Bomb
    fire_grenade_timer += dt;
    if (fire_grenade_timer >= 5.0f && dist < 700.0f) {
        fire_grenade_timer = 0.0f;

        float vx = (dx > 0.0f) ? 4.5f : -4.5f; Projectile* p = new Projectile(  pos_x + (float)enemy_width * 0.5f, pos_y,  vx, -9.0f, 20.0f, 16, 16 );
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setisFire(true);   
        p->setSpriteTexture(fire_grenade_tex_boss);
        addToPool(p, pool, proj_count, max_proj);
    }
  }
void IronNokana::render(sf::RenderWindow& window, float cam_x, float cam_y) {

    if (!is_alive) 
      return;

    //sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));//anime 

    float sx = (float)enemy_width / (float)frame_width;   
    float sy = (float)enemy_height / (float)frame_height; 

    if (velocity_x < 0.0f || is_retreating) {
        sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(pos_x - cam_x + (float)enemy_width, pos_y - cam_y);
    }
    else {
        sprite.setScale(-1.0f, 1.0f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    }


    window.draw(sprite);
}
void HairbusterRiberts::move(Character* player, Level* level, float dt) {
    if (!is_retreating && hp <= 15.0f)
        is_retreating = true;

    if (is_retreating) {
        velocity_x = speed * 4.0f;
        pos_x += velocity_x;
        if (pos_x > (float)(level->getWidth() * level->getCellSize()) + 64.0f) {
            has_retreated = true;
            is_alive = false;
        }
        return;
    }
    const float RISE = 150.0f;
    if (pos_y > hover_y + 2.0f)
        pos_y -= RISE * dt;
    else if (pos_y < hover_y - 2.0f) 
       pos_y += RISE * dt;

    patrol_timer += dt;
    if (patrol_timer >= 3.5f) {
        patrol_timer = 0.0f;
        patrol_dir = -patrol_dir;
    }

    float dx = player->getPosX() - pos_x;
    float drift = (dx > 0.0f ? 0.35f : -0.35f) * speed;
    velocity_x = patrol_dir * speed * 0.6f + drift;
    velocity_y = 0.0f; //no gravity,pos_y manipulated

}

void HairbusterRiberts::attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bullet_tex, float dt) {
    if (is_retreating)  
      return;

      float dx = player->getPosX()-pos_x;
      float dy = player->getPosY()-pos_y;
      float dist = sqrtf(dx * dx + dy * dy);
      if (dist < 1.0f) 
        dist = 1.0f;

        //Rocket
      rocket_timer += dt;
      if (rocket_timer >= 2.0f && dist < 950.0f) {
          rocket_timer = 0.0f;

          for (int spread = -1; spread <= 1; spread += 2) {
          float vx = (dx / dist) * 6.0f + (float)spread * 1.5f;
          float vy = (dy / dist) * 6.0f;
          Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y + (float)enemy_height, vx, vy, 5.0f, 24, 12);
          p->setFromEnemy(true);
          p->setExplosive(true, 3.0f);
          p->setGrenade(true);
          p->setSpriteTexture(rocket_tex_boss);
          addToPool(p, pool, proj_count, max_proj);
          }
}
      
      //Fire Bomb
      fire_grenade_timer += dt;
      if (fire_grenade_timer >= 5.0f && dist < 700.0f) {
          fire_grenade_timer = 0.0f;

          float vx = (dx > 0.0f) ? 3.5f : -3.5f; 
          Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, 4.0f, 20.0f, 16, 16);
          p->setFromEnemy(true);
          p->setExplosive(true, 3.0f);
          p->setGrenade(true);
          p->setisFire(true);
          p->setSpriteTexture(fire_grenade_tex_boss);
          addToPool(p, pool, proj_count, max_proj);
      }
}

void HairbusterRiberts::render(sf::RenderWindow& window, float cam_x, float cam_y)  {
    if (!is_alive) 
      return;
  //  sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width*2, frame_height*2));//anime


    if (velocity_x < 0.0f || is_retreating) {
        sprite.setScale(-1.0f, 1.0f);
        sprite.setPosition(pos_x - cam_x + (float)enemy_width, pos_y - cam_y);
    }
    else {
        sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    }

    animateSprite(sprite, animation_timer, current_frame,
        0, 45,12,  frame_width, frame_height, 1.0f / 60.0f);


    window.draw(sprite);
}

void SeaSatan::move(Character* player, Level* level, float dt) {
    if (!is_retreating && hp <= 15.0f)
        is_retreating = true;

    if (is_retreating) {
        velocity_x = speed * 4.0f;
        pos_x += velocity_x;
        if (pos_x > (float)(level->getWidth() * level->getCellSize()) + 64.0f) {
            has_retreated = true;
            is_alive = false;
        }
        return;
    }
    float dx = player->getPosX() - pos_x;
    float abs_dx = (dx >= 0.0f) ? dx : -dx;

    if (abs_dx > 320.0f) {
        velocity_x = (dx > 0.0f) ? speed : -speed;
    }
    else {
        patrol_timer += dt;
        if (patrol_timer >= 4.0f) {
            patrol_timer = 0.0f;
            patrol_dir = -patrol_dir;
        }
        velocity_x = patrol_dir * speed * 0.5f;
    }
    velocity_y = 0.0f; 

} 
void SeaSatan::attack(Character* player, Projectile** pool, int& proj_count, int max_proj, Texture& bullet_tex, float dt) {
    if (is_retreating) 
      return;

    float dx = player->getPosX() - pos_x;
    float dy = player->getPosY() - pos_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f) 
      dist = 1.0f;
      //machine gun
    hmg_timer += dt;
    if (hmg_timer >= 0.25f && dist < 800.0f) {
        hmg_timer = 0.0f;
        float gun_y = pos_y + (float)enemy_height * 0.3f;
       // LEFT GUN
        Projectile* pl = new Projectile(pos_x, gun_y, -8.0f, 0.0f, 3.0f, 16, 8);
        pl->setFromEnemy(true);
        pl->setSpriteTexture(bullet_tex_boss);
        addToPool(pl, pool, proj_count, max_proj);
        //RIGHT GUN
        Projectile* pr = new Projectile(pos_x + (float)enemy_width, gun_y, 8.0f, 0.0f, 3.0f, 16, 8);
        pr->setFromEnemy(true);
        pr->setSpriteTexture(bullet_tex_boss);
        addToPool(pr, pool, proj_count, max_proj);  
    }
    //Rocket
    rocket_timer += dt;
    if (rocket_timer >= 3.0f && dist < 900.0f) {
        rocket_timer = 0.0f;

        float vx = (dx / dist) * 7.0f;
        float vy = (dy / dist) * 7.0f;

        Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, vy, 5.0f, 24, 12);
        p->setFromEnemy(true);
        p->setExplosive(true, 3.0f);
        p->setGrenade(true);
        p->setSpriteTexture(rocket_tex_boss);
        addToPool(p, pool, proj_count, max_proj);



    }
}
void SeaSatan::render(RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive)
      return;
    
    if (velocity_x < 0.0f || is_retreating) {
        sprite.setScale(-4.0f, 4.0f);
        sprite.setPosition(pos_x - cam_x + (float)enemy_width, pos_y - cam_y);
    }
    else {
        sprite.setScale(4.0f, 4.0f);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    }
    updateDamageFrame();   // states f le dmg
    sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, frame_height));
    window.draw(sprite);

}


void UltimateBoss::render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_alive) return;
    float abs_sx = (sprite.getScale().x < 0) ? -sprite.getScale().x : sprite.getScale().x;
    float abs_sy = (sprite.getScale().y < 0) ? -sprite.getScale().y : sprite.getScale().y;

    if (velocity_x < 0.0f) {
        sprite.setScale(-abs_sx, abs_sy);
        sprite.setPosition(pos_x - cam_x + enemy_width, pos_y - cam_y);
    }
    else {
        sprite.setScale(abs_sx, abs_sy);
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    }

    window.draw(sprite);
    }
void UltimateBoss::attack(Character* player, Projectile** pool, int& proj_count, int max_proj, sf::Texture& bt, float dt) {
    if (!is_alive)
      return;

    float dx = player->getPosX() - pos_x;
    float dy = player->getPosY() - pos_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f)
      dist = 1.0f;

    if (state == 0 && ground_hp > 0.0f) {
        //Missiles
        missile_timer_g += dt;
        if (missile_timer_g >= 2.5f && dist < 900.0f) {
            missile_timer_g = 0.0f;

            float vx = (dx > 0.0f) ? 5.0f : -5.0f;
            Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, -10.0f, 5.0f, 24, 12
            );
            p->setFromEnemy(true);
            p->setExplosive(true, 3.0f);
            p->setGrenade(true);
            p->setSpriteTexture(missile_tex);
            addToPool(p, pool, proj_count, max_proj);
        }
        //Flames
        flame_timer_g += dt;
        if (flame_timer_g >= 3.0f && dist < 350.0f) {
            flame_timer_g = 0.0f;

            float flame_x = (dx > 0.0f) ? 6.0f : -6.0f;
            float sx = pos_x + (dx > 0.0f ? (float)enemy_width : 0.0f);
            float sy = pos_y + (float)enemy_height - 15.0f;
            for (int i = 0; i < 6; i++) {
                Projectile* p = new Projectile(sx + flame_x * (float)i * 0.8f, sy, flame_x, 0.0f, 2.0f / 30.0f, 32, 1);
                p->setFromEnemy(true);
                p->setisFire(true);
                p->setLifespan(1.2f);
                p->setSpriteTexture(flame_tex);
                addToPool(p, pool, proj_count, max_proj);

            }

        }
        //Fire Bomb
        fire_grenade_timer_g += dt;
        if (fire_grenade_timer_g >= 5.0f && dist < 700.0f) {
            fire_grenade_timer_g = 0.0f;

            float vx = (dx > 0.0f) ? 4.5f : -4.5f; Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, -9.0f, 20.0f, 16, 16);
            p->setFromEnemy(true);
            p->setExplosive(true, 3.0f);
            p->setGrenade(true);
            p->setisFire(true);
            p->setSpriteTexture(fg_tex);
            addToPool(p, pool, proj_count, max_proj);
        }

    }
    else if (state == 1 && aerial_hp > 0.0f) {
        //Rocket
        rocket_timer_a += dt;
        if (rocket_timer_a >= 2.0f && dist < 950.0f) {
            rocket_timer_a = 0.0f;

            for (int spread = -1; spread <= 1; spread += 2) {
                float vx = (dx / dist) * 6.0f + (float)spread * 1.5f;
                float vy = (dy / dist) * 6.0f;
                Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y + (float)enemy_height, vx, vy, 5.0f, 24, 12);
                p->setFromEnemy(true);
                p->setExplosive(true, 3.0f);
                p->setGrenade(true);
                p->setSpriteTexture(rocket_tex);
                addToPool(p, pool, proj_count, max_proj);
            }
        }

        //Fire Bomb
        fire_grenade_timer_a += dt;
        if (fire_grenade_timer_a >= 5.0f && dist < 700.0f) {
            fire_grenade_timer_a = 0.0f;

            float vx = (dx > 0.0f) ? 3.5f : -3.5f;
            Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, 4.0f, 20.0f, 16, 16);
            p->setFromEnemy(true);
            p->setExplosive(true, 3.0f);
            p->setGrenade(true);
            p->setisFire(true);
            p->setSpriteTexture(fg_tex);
            addToPool(p, pool, proj_count, max_proj);
        }

    }
    else if (state == 2 && aquatic_hp > 0.0f) {
        //machine gun
        hmg_timer_q += dt;
        if (hmg_timer_q >= 0.25f && dist < 800.0f) {
            hmg_timer_q = 0.0f;
            float gun_y = pos_y + (float)enemy_height * 0.3f;
            // LEFT GUN
            Projectile* pl = new Projectile(pos_x, gun_y, -8.0f, 0.0f, 3.0f, 16, 8);
            pl->setFromEnemy(true);
            pl->setSpriteTexture(bullet_tex_u);
            addToPool(pl, pool, proj_count, max_proj);
            //RIGHT GUN
            Projectile* pr = new Projectile(pos_x + (float)enemy_width, gun_y, 8.0f, 0.0f, 3.0f, 16, 8);
            pr->setFromEnemy(true);
            pr->setSpriteTexture(bullet_tex_u);
            addToPool(pr, pool, proj_count, max_proj);
        }
        //Rocket
        rocket_timer_q += dt;
        if (rocket_timer_q >= 3.0f && dist < 900.0f) {
            rocket_timer_q = 0.0f;

            float vx = (dx / dist) * 7.0f;
            float vy = (dy / dist) * 7.0f;

            Projectile* p = new Projectile(pos_x + (float)enemy_width * 0.5f, pos_y, vx, vy, 5.0f, 24, 12);
            p->setFromEnemy(true);
            p->setExplosive(true, 3.0f);
            p->setGrenade(true);
            p->setSpriteTexture(rocket_tex);
            addToPool(p, pool, proj_count, max_proj);



        }

    }
}
void UltimateBoss::move(Character* player, Level* level, float dt) {
    if (!is_alive)
      return;

    state_timer -= dt;
    if (state_timer <= 0.0f)
        tryNextState();

    Texture* tex = nullptr;
    if (state == 0)
       tex = &ground_tex;
    else if (state == 1) 
      tex = &aerial_tex;
    else                
      tex = &aquatic_tex;

    if (tex) {
        sprite.setTexture(*tex);
        sprite.setScale((float)enemy_width / (float)tex->getSize().x, (float)enemy_height / (float)tex->getSize().y);
    }
    float dx = player->getPosX() - pos_x;
    float abs_dx = (dx >= 0.0f) ? dx : -dx;
    float effective_speed = speed * power_boost;
    if (state == 0) {
        if (abs_dx > 380.0f)
            velocity_x = (dx > 0.0f) ? effective_speed : -effective_speed;
        else if (abs_dx < 200.0f)
            velocity_x = (dx > 0.0f) ? -effective_speed * 0.4f : effective_speed * 0.4f;
        else
            velocity_x = 0.0f;

        checkAndJump(level);

    }
    else if (state == 1) {
        const float RISE = 150.0f;
        if (pos_y > hover_y + 2.0f)
            pos_y -= RISE * dt;
        else if (pos_y < hover_y - 2.0f)
            pos_y += RISE * dt;

        patrol_timer_a += dt;
        if (patrol_timer_a >= 3.5f) {
            patrol_timer_a = 0.0f;
            patrol_dir_a = -patrol_dir_a;
        }

        float dx = player->getPosX() - pos_x;
        float drift = (dx > 0.0f ? 0.35f : -0.35f) * speed;
        velocity_x = patrol_dir_a * speed * 0.6f + drift;
        velocity_y = 0.0f; //no gravity,pos_y manipulated
    }
    else {
        const float SINK_SPEED = 100.0f;

        if (pos_y > water_y + 2.0f) {
            pos_y -= SINK_SPEED * dt;
        }
        else if (pos_y < water_y - 2.0f) {
            pos_y += SINK_SPEED * dt;
        }

        patrol_timer_q += dt;

        if (patrol_timer_q >= 4.0f) {
            patrol_timer_q = 0.0f;
            patrol_dir_q = -patrol_dir_q;
        }

        if (abs_dx > 320.0f) {
            velocity_x = (dx > 0.0f) ? effective_speed: -effective_speed;
        }
        else {
            velocity_x = patrol_dir_q * effective_speed * 0.5f;
        }

        velocity_y = 0.0f;
    }
}
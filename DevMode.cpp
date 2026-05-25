#include "DevMode.h"


DevMode::DevMode() {
active = false;
key_press = false;
key_count = 0;
key_timer =0.0f;
}
bool DevMode::isActive() const {
    return active;
}
bool DevMode::handleInput(bool d_key_now, float dt, Character** characters, int num_chars) {
    if (key_timer > 0.0f) {
        key_timer -= dt;
        if (key_timer <= 0.0f) {
            key_timer = 0.0f;
            key_count = 0;
           }
   }

   bool toggle = false;

   if (d_key_now && !key_press) {
       if (key_timer > 0.0f) {
           key_count++;

      }
       else {
           key_count = 1;
           key_timer = 2.0f;
       }

       if (key_count >= 3) {
           key_count = 0;
           key_timer =0.0f;

           if (active) {
               removeEffects(characters, num_chars);
            }
           else {
               applyEffects(characters, num_chars);
           }

           active = !active;
           toggle = true;
        }
      }

   key_press = d_key_now;
   return toggle;
}
void DevMode::applyEffects(Character** characters, int num_chars) {
    for (int i = 0; i < num_chars; i++)
        characters[i]->setDevImmortal(true);
  
    Weapon* dev_weapons[4] = {        
    new HeavyMachineGun(),new RocketLauncher(),new FlameShot(),new LaserGun()
    };

    for (int i = 0; i < num_chars && i < 4; i++) {
     if(!characters)
      continue;
        characters[i]->setHP(100.0f);
        characters[i]->resetState();
        characters[i]->setGrenadeCount(999);
        Weapon* old = characters[i]->getCurrentWeapon();
        if (old)
           delete old;
        characters[i]->setWeapon(dev_weapons[i]);
    }

}
void DevMode::removeEffects(Character** characters, int num_chars) {
    for (int i = 0; i < num_chars; i++)
        characters[i]->setDevImmortal(false);

    for (int i = 0; i < num_chars && i < 4; i++) {
        if (!characters)
            continue;
        characters[i]->setGrenadeCount(characters[i]->get_old_grenade_count());
        Weapon* old = characters[i]->getCurrentWeapon();
        if (old)
            delete old;
        characters[i]->setWeapon(new Pistol());
    }

}

void DevMode::renderHUD(sf::RenderWindow& window, sf::Text& mode_text) {
    if (!active) 
       return;

    mode_text.setString("DEV MODE ON");
    mode_text.setFillColor(sf::Color::Red);
    mode_text.setPosition(700, 10);
    window.draw(mode_text);
    mode_text.setFillColor(sf::Color::Yellow);
}

void DevMode::reset(Character** characters, int num_chars) {
    if (active) removeEffects(characters, num_chars);
    active = false; key_count = 0; key_timer = 0.0f;
}
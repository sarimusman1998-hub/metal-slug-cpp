#include "InteractableObject.h"

void InteractableObject::takeDamage(float) {
}

void InteractableObject::render(sf::RenderWindow& window, float cam_x, float cam_y) {
    if (!is_active) {
        return;
    }

    sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
    window.draw(sprite);
}

POWPrisoner::POWPrisoner(float x, float y) : InteractableObject(x, y), is_freed(false) {

    texture.loadFromFile("25I-0555_25I-1034_Assets/pow_prisoner.png");
    sprite.setTexture(texture);
    sprite.setScale(64.0f / (float)texture.getSize().x, 64.0f / (float)texture.getSize().y);

    float sx = 32.0f / (float)texture.getSize().x;
    float sy = 48.0f / (float)texture.getSize().y;

    sprite.setScale(sx, sy);
}

void POWPrisoner::onInteract() {
    is_freed = true;
    is_active = false;
}

SupplyCrate::SupplyCrate(float x, float y, int grenades, int wp_type)  : InteractableObject(x, y), grenade_count(grenades) {

    if (wp_type < 0) {
        int roll = rand() % 10;

        if (roll < 3) {
            weapon_type = 0;
        }
        else if (roll < 6) {
            weapon_type = 1;
        }
        else if (roll < 9) {
            weapon_type = 2;
        }
        else {
            weapon_type = 3;
        }
    }
    else {
        weapon_type = wp_type;
    }

    switch (weapon_type) {
    case 0:
        ammo_amount = 100;
        break;

    case 1:
        ammo_amount = 200;
        break;

    case 2:
        ammo_amount = 20;
        break;

    case 3:
        ammo_amount = 10;
        break;

    default:
        ammo_amount = 0;
        break;
    }

    grenade_count = grenades;

    texture.loadFromFile("25I-0555_25I-1034_Assets/supply_crate.png");
    sprite.setTexture(texture);
    sprite.setScale(64.0f / (float)texture.getSize().x, 64.0f / (float)texture.getSize().y);

    float sx = 32.0f / (float)texture.getSize().x;
    float sy = 32.0f / (float)texture.getSize().y;

    sprite.setScale(sx, sy);
}

void SupplyCrate::onInteract() {
    is_active = false;
}
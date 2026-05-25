#pragma once
#include "DamagableEntity.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>

class InteractableObject : public DamagableEntity {
protected:
    float pos_x, pos_y;
    bool is_active;

public:
    sf::Sprite sprite;
    sf::Texture texture;

    float getPosX() const {
        return pos_x;
    }

    float getPosY() const {
        return pos_y;
    }

    bool getIsActive() const {
        return is_active;
    }

    InteractableObject(float x, float y)
        : pos_x(x), pos_y(y), is_active(true) {

    }

    virtual void onInteract() = 0;

    void takeDamage(float) override;

    void render(sf::RenderWindow& window, float cam_x, float cam_y) override;

    virtual ~InteractableObject() {

    }
};

class POWPrisoner : public InteractableObject {
protected:
    bool is_freed;

public:
    POWPrisoner(float x, float y);

    void onInteract() override;

    bool getIsFreed() const {
        return is_freed;
    }
};

class SupplyCrate : public InteractableObject {
protected:
    int grenade_count;
    int weapon_type;
    int ammo_amount;

public:
    SupplyCrate(float x, float y, int grenades = 4, int wp_type = -1);

    int getGrenadeCount() const {
        return grenade_count;
    }

    int getWeaponType() const {
        return weapon_type;
    }

    int getAmmo() const {
        return ammo_amount;
    }

    Weapon* createWeapon() const {
        switch (weapon_type) {
        case 0:
            return new HeavyMachineGun();

        case 1:
            return new FlameShot();

        case 2:
            return new RocketLauncher();

        case 3:
            return new LaserGun();

        default:
            return new Pistol();
        }
    }

    void onInteract() override;
};
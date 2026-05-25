#pragma once
#include "DamagableEntity.h"
#include <SFML/Graphics.hpp>

class Collectible : public DamagableEntity {
protected:
    const char* type;     
    int   heal_value;
    bool  is_collected;
    float   pos_x, pos_y;
    public:
    Sprite  sprite;
    Texture texture;
    int getHealValue() const { 
      return heal_value; 
      }
    void collect() { 
     is_collected = true;
     }
    bool isCollected() const {
        return is_collected;
    }
    float getPosX() const {
        return pos_x;
    }
    float getPosY() const {
        return pos_y;
    }
    Collectible(const char* t, int val, float x, float y) : type(t), heal_value(val), is_collected(false), pos_x(x), pos_y(y){

    }
    void takeDamage(float) override {

    }

    void render(sf::RenderWindow& window, float cam_x, float cam_y) override {
        if (is_collected) return;
        sprite.setPosition(pos_x - cam_x, pos_y - cam_y);
        window.draw(sprite);
    }


    virtual ~Collectible() {
    
    }
};
class FruitCollectible : public Collectible {
public:
    FruitCollectible(float x, float y) : Collectible("fruit", 2, x, y) {
        texture.loadFromFile("Assets/fruit.png");
        sprite.setTexture(texture);
        sprite.setScale(64.0f / (float)texture.getSize().x, 64.0f / (float)texture.getSize().y);
    }
};
class TurkeyCollectible : public Collectible {
public:
    TurkeyCollectible(float x, float y) : Collectible("turkey", 3, x, y) {
        texture.loadFromFile("Assets/turkey.png");
        sprite.setTexture(texture);
        sprite.setScale(64.0f / (float)texture.getSize().x, 64.0f / (float)texture.getSize().y);
    }
};
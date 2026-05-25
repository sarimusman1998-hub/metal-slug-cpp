#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    virtual ~Entity() {}
    virtual void render(sf::RenderWindow& window, float cam_x, float cam_y) = 0;
};

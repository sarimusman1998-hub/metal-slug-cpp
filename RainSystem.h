#pragma once
#include "Level.h"
#include <SFML/Graphics.hpp>

class Level;


class RainSystem {
private:
const int MAX_DROPS = 60;
float * drop_x;
float * drop_y;
sf::Texture rain_tex;
sf::Sprite  rain_sprite;
bool is_raining;
float rain_timer;
float next_rain;
float scan_timer;
public:
	RainSystem(sf::Texture& drop_tex)  {
        is_raining = false;
        rain_timer = 0.0f;
        next_rain = 1.0f;            
        scan_timer = 1.0f;
        drop_x = new float[MAX_DROPS];
        drop_y = new float[MAX_DROPS];
        for (int i = 0; i < MAX_DROPS; i++) {
            drop_x[i] = (float)(rand() % 1600);
            drop_y[i] = (float)(rand() % 900);
        }
        rain_sprite.setTexture(drop_tex);
        rain_sprite.setScale(1.0f, 1.0f);
}
    bool getIsRaining() const {
       return is_raining;
       }
    void update(float dt, Level* level) {
        if (!is_raining) {
            next_rain -=dt;
            if (next_rain <= 0.0f) {
                is_raining = true;
                rain_timer = 5.0f + (float)(rand() % 8);
                next_rain = 25.0f + (float)(rand() % 35);
              }
              return;
       }

        rain_timer -= dt;
        if (rain_timer <= 0.0f) {
            is_raining = false;
            return;
        }

        for (int i = 0; i < MAX_DROPS; i++) {
            drop_y[i] += 600.0f * dt;
            if (drop_y[i] > 920.0f) {
                drop_y[i] = -10.0f;
                drop_x[i] = (float)(rand() % 1600);
            }
        }

        scan_timer -= dt;
        if (scan_timer <= 0.0f) {
            scan_timer = 1.5f;
            for (int r = 0;r < level->getHeight();r++) {
                for (int c = 0; c < level->getWidth(); c++) {
                    if (level->getBlock(r, c) == ' ')
                        level->checkCraterWater(r, c);
                }
            }

        }
    }
    void render(sf::RenderWindow& window, float cam_x, float cam_y) {
        if (!is_raining) return;
        for (int i = 0; i < MAX_DROPS; i++) {
            rain_sprite.setPosition(drop_x[i], drop_y[i]);  
            window.draw(rain_sprite);
        }
    }

    ~RainSystem() {
        delete[] drop_x;
        delete[] drop_y;
    }
};
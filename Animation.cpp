#include <SFML/Graphics.hpp>
#include "Animation.h"
void  animateSprite(sf::Sprite& sprite, float& time, int& currentFrame, int startFrame, int frameCount,
	float fps, int frameWidth, int frameHeight, float deltaTime)
{
    if (currentFrame < startFrame || currentFrame >= startFrame + frameCount)
    {
        currentFrame = startFrame;
        time = 0.0f;
    }
    time += deltaTime;

    if (time >= 1.0f / fps)
    {
        time = 0.0f;

        currentFrame++;

        if (currentFrame >= startFrame + frameCount)
            currentFrame = startFrame;

        int left = currentFrame * frameWidth;

        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    
    }

}

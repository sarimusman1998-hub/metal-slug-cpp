#pragma once
#include "Entity.h"

class DamagableEntity : public Entity {
public:
    virtual void takeDamage(float dmg) = 0;
    virtual ~DamagableEntity() {
    
    }
};

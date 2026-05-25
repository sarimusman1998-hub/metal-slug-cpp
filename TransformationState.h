#pragma once
class TransformationState {
public:

    virtual int getStateID() const = 0;
    virtual void onTick(float dt) {
    
    }
    virtual float getSpeedMultiplier() const {
        return 1.0f;
    }

    virtual bool allowsShooting() const {
        return true;
    }
    virtual void onExit() {
    
    }
    virtual ~TransformationState() {
    
    }
};

class NormalTransformation : public TransformationState {
public:
    int getStateID() const override {
    
    return 0;
    
    }  
};

class ZombieTransformation : public TransformationState {
public:
    int getStateID() const override { 
    
    return 1;
    
    } 
    float getSpeedMultiplier() const override {
        return 0.5f; 
        }
};

class MummyTransformation : public TransformationState {
public:
    int getStateID() const override { 
    
    return 2;
    
    } 
    float getSpeedMultiplier() const override { 
         return 0.65f;
         }
    bool  allowsShooting()     const override { 
         return false;
         }
};
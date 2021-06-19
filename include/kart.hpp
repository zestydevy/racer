#pragma once

#include <nitro.h>
#include "actor.hpp"
#include "gamepad.hpp"

class TKart : public TActor
{
    public:
    TKart(TGamepad * gamepad) : TActor(){ mGamepad = gamepad; };
    ~TKart(){};

    virtual void update();
    virtual void draw();

    protected:
    TGamepad * mGamepad; 

    fx32 mMaxSpeed;
    fx32 mAccel;
    fx32 mTurnRate;

    fx32 mSpeed;
    fx32 mRotation;
};
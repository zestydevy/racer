#pragma once

#include <nitro.h>
#include "actor.hpp"
#include "gamepad.hpp"

class TKart : public TActor
{
    public:
    TKart(TGamepad * gamepad);
    ~TKart(){};

    virtual void update();
    virtual void draw();

    s16 getDirection(){ return mDirection; }

    protected:
    TGamepad * mGamepad; 

    fx32 mMaxSpeed;
    fx32 mAccel;
    s16 mTurnRate;

    fx32 mSpeed;
    s16 mDirection;

    s16 mSteer;
};
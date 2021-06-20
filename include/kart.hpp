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
    s16 getFacingDirection(){ return mDirection - mVisibleDirection; }
    fx32 getSpeed(){ return mSpeed; }

    protected:
    TGamepad * mGamepad; 

    bool mDrifting;
    bool mDriftDirection;

    fx32 mMaxSpeed;
    fx32 mAccel;
    s16 mTurnRate;
    s16 mTurnRateDrift;

    fx32 mSpeed;
    fx32 mVSpeed;
    s16 mDirection;
    s16 mVisibleDirection;

    s16 mSteer;

    private:
    fx32 getGroundHeight();
};
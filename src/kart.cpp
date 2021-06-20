#include "kart.hpp"

#include "g3d_demolib.h"

const fx32 GRAVITY = FX_F32_TO_FX32(1.0f);

const s16 STEER_MAX = 196;

TKart::TKart(TGamepad* gamepad){
    mGamepad = gamepad;
    mMaxSpeed = FX_F32_TO_FX32(1.0f);
    mAccel = FX_F32_TO_FX32(0.02f);
    mTurnRate = 8;
    mDirection = 0;
    mSteer = 0;
    mSpeed = 0;

    mPosition.x = 0;
    mPosition.y = 0;
    mPosition.z = 0;

    mScale.x = FX32_ONE;
    mScale.y = FX32_ONE;
    mScale.z = FX32_ONE;
}

void TKart::update()
{
    //moving with acceleration
    if (mGamepad->checkHeld(PAD_BUTTON_A)){
        mSpeed += mAccel;
        if (mSpeed > mMaxSpeed)
            mSpeed = mMaxSpeed;
    }
    else if (mGamepad->checkHeld(PAD_BUTTON_B)){
        mSpeed -= mAccel;
        if (mSpeed < -mMaxSpeed)
            mSpeed = -mMaxSpeed;
    }
    else if (mSpeed > 0) {
        if (mSpeed < mAccel * 2)
            mSpeed = 0;
        else
            mSpeed -= mAccel * 2;
    }
    else if (mSpeed < 0){
        if (mSpeed > -mAccel * 2)
            mSpeed = 0;
        else
            mSpeed += mAccel * 2;
    }

    //steering with moment
    if (mGamepad->checkHeld(PAD_KEY_RIGHT)){
        mSteer += mTurnRate;
        if (mSteer > STEER_MAX)
            mSteer = STEER_MAX;
    }
    else if (mGamepad->checkHeld(PAD_KEY_LEFT)){
        mSteer -= mTurnRate;
        if (mSteer < -STEER_MAX)
            mSteer = -STEER_MAX;
    }
    else if (mSteer > 0){
        if (mSteer < mTurnRate)
            mSteer = 0;
        else
            mSteer -= mTurnRate;
    }
    else if (mSteer < 0){
        if (mSteer > -mTurnRate)
            mSteer = 0;
        else
            mSteer += mTurnRate;
    }

    mDirection -= mSteer;

    mPosition.z += mSpeed * FX_CosIdx((u16)mDirection) >> FX32_SHIFT;
    mPosition.x += mSpeed * FX_SinIdx((u16)mDirection) >> FX32_SHIFT;
}

void TKart::draw()
{
    MtxFx33 rot;
    MTX_RotY33(&rot, FX_SinIdx((u16)mDirection), FX_CosIdx((u16)mDirection));

    NNS_G3dGlbSetBaseScale(&mScale);
    NNS_G3dGlbSetBaseTrans(&mPosition);
    NNS_G3dGlbSetBaseRot(&rot);
    NNS_G3dGlbFlushP();
    NNS_G3dDraw(&mRenderObj);
}
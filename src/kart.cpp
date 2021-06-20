#include "kart.hpp"

#include "g3d_demolib.h"

const fx32 GRAVITY = FX_F32_TO_FX32(0.2f);

const s16 MIN_DRIFT_SPEED = FX_F32_TO_FX32(0.5f);
const s16 STEER_MAX = 196;
const s16 STEER_MAX_DRIFT = 256;

/* Drift animation rates and amounts */
const s16 DRIFT_ANIM_START = 196;
const s16 DRIFT_ANIM_END = 512;
const s16 DRIFT_ANIM_LIGHT = 0x1200;
const s16 DRIFT_ANIM_NEUTRAL = 0x1700;
const s16 DRIFT_ANIM_HARD = 0x2000;

#define PAD_STEER_RIGHT (PAD_KEY_RIGHT | PAD_BUTTON_A)
#define PAD_STEER_LEFT (PAD_KEY_LEFT | PAD_BUTTON_Y)
#define PAD_BRAKE (PAD_BUTTON_L | PAD_BUTTON_R)

TKart::TKart(TGamepad* gamepad){
    mGamepad = gamepad;
    mMaxSpeed = FX_F32_TO_FX32(2.0f);
    mAccel = FX_F32_TO_FX32(0.01f);
    mTurnRate = 8;
    mTurnRateDrift = 4;
    mDirection = 0;
    mSteer = 0;
    mSpeed = 0;
    mVSpeed = 0;
    mDrifting = false;
    mVisibleDirection = 0;

    mPosition.x = 0;
    mPosition.y = 0;
    mPosition.z = 0;

    mScale.x = FX32_ONE;
    mScale.y = FX32_ONE;
    mScale.z = FX32_ONE;
}

void TKart::update()
{
    s16 effectiveTurnRate = mTurnRate;
    s16 effectiveMaxSteer = STEER_MAX;

    //Check if brake buttons were just pressed
    if (mGamepad->checkPressed(PAD_BRAKE) && mPosition.y <= getGroundHeight()){
        if (mGamepad->checkHeld(PAD_STEER_LEFT | PAD_STEER_RIGHT) && mSpeed >= MIN_DRIFT_SPEED) {    //if steering while pressing brake, start drift
            mDrifting = true;
            mDriftDirection = mGamepad->checkHeld(PAD_KEY_RIGHT);
        }
        mVSpeed = GRAVITY * 5;
    }
    //Check if brake buttons are held
    if (mGamepad->checkHeld(PAD_BRAKE)){
        //Check if drifting
        if (!mDrifting){ 

            if (mPosition.y <= getGroundHeight()){
                //Apply deceleration
                mSpeed -= mAccel;
                if (mSpeed < -mMaxSpeed)
                    mSpeed = -mMaxSpeed;
            }
        }
        else{ //Drift logic
            if ((mDriftDirection && mSteer <= 0) || (!mDriftDirection && mSteer >= 0) || (mSpeed < MIN_DRIFT_SPEED))  //Stop drift if player changes direction or slows too much
                mDrifting = false;

            if (mPosition.y <= getGroundHeight()){
                //Apply normal acceleration
                mSpeed += mAccel;
                if (mSpeed > mMaxSpeed)
                    mSpeed = mMaxSpeed;
            }
        }
    }
    else{ //Brake buttons are not pressed or held, auto accelerate
        //Brake buttons are not held, so we stop drifting
        if (mDrifting)
            mDrifting = false;

        //Apply normal acceleration
        mSpeed += mAccel;
        if (mSpeed > mMaxSpeed)
            mSpeed = mMaxSpeed;
    }

    //Apply drift steering
    if (mDrifting){
        s16 driftAmount = DRIFT_ANIM_NEUTRAL;
        effectiveTurnRate = mTurnRateDrift;
        effectiveMaxSteer = STEER_MAX_DRIFT;

        if ((mGamepad->checkHeld(PAD_STEER_RIGHT) && mDriftDirection) || (mGamepad->checkHeld(PAD_STEER_LEFT) && !mDriftDirection))
            driftAmount = DRIFT_ANIM_HARD;
        else if ((mGamepad->checkHeld(PAD_STEER_RIGHT) && !mDriftDirection) || (mGamepad->checkHeld(PAD_STEER_LEFT) && mDriftDirection))
            driftAmount = DRIFT_ANIM_LIGHT;

        if (!mDriftDirection)
            driftAmount = -driftAmount;

        //Apply drift animation
        if (mVisibleDirection < driftAmount){
            mVisibleDirection += DRIFT_ANIM_START;
        }
        else if (mVisibleDirection > driftAmount){
            mVisibleDirection -= DRIFT_ANIM_START;
        }
    }
    else{
        //Stop drift animation
        if (mVisibleDirection > 0){
            if (mVisibleDirection < DRIFT_ANIM_END)
                mVisibleDirection = 0;
            else
                mVisibleDirection -= DRIFT_ANIM_END;
        }
        else if (mVisibleDirection < 0){
            if (mVisibleDirection > -DRIFT_ANIM_END)
                mVisibleDirection = 0;
            else
                mVisibleDirection += DRIFT_ANIM_END;
        }
    }

    //steering with moment
    if (mGamepad->checkHeld(PAD_STEER_RIGHT)){
        mSteer += effectiveTurnRate;
        if (mSteer > effectiveMaxSteer)
            mSteer = effectiveMaxSteer;
    }
    else if (mGamepad->checkHeld(PAD_STEER_LEFT)){
        mSteer -= effectiveTurnRate;
        if (mSteer < -effectiveMaxSteer)
            mSteer = -effectiveMaxSteer;
    }
    else if (mSteer > 0){
        if (mSteer < effectiveTurnRate)
            mSteer = 0;
        else
            mSteer -= effectiveTurnRate;
    }
    else if (mSteer < 0){
        if (mSteer > -effectiveTurnRate)
            mSteer = 0;
        else
            mSteer += effectiveTurnRate;
    }

    //Apply gravity
    if (mPosition.y > getGroundHeight())
        mVSpeed -= GRAVITY;
    else{
        if (mVSpeed < GRAVITY * -6){    //Bounce
            mVSpeed /= -3;
        }
        else if (mVSpeed < 0)
            mVSpeed = 0;

        mPosition.y = getGroundHeight();
    }

    if (mPosition.y <= getGroundHeight()){
        if (mSpeed < 0)
            mDirection += mSteer;
        else
            mDirection -= mSteer;
    }

    mPosition.z += mSpeed * FX_CosIdx((u16)mDirection) >> FX32_SHIFT;
    mPosition.x += mSpeed * FX_SinIdx((u16)mDirection) >> FX32_SHIFT;
    mPosition.y += mVSpeed;
}

void TKart::draw()
{
    MtxFx33 rot;
    MTX_RotY33(&rot, FX_SinIdx((u16)(mDirection - mVisibleDirection)), FX_CosIdx((u16)(mDirection - mVisibleDirection)));

    NNS_G3dGlbSetBaseScale(&mScale);
    NNS_G3dGlbSetBaseTrans(&mPosition);
    NNS_G3dGlbSetBaseRot(&rot);
    NNS_G3dGlbFlushP();
    NNS_G3dDraw(&mRenderObj);
}

fx32 TKart::getGroundHeight(){
    return 0;
}
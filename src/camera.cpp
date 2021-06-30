#include "camera.hpp"

#define	CAMERA_SPEED  		256

#define	MAX_ROTATION		0x3f00				/* 90deg == 0x4000*/
#define	MIN_ROTATION		(-MAX_ROTATION)

#define	MAX_DISTANCE		(150 * FX32_ONE)
#define	MIN_DISTANCE		(  4 * FX32_ONE)

TCamera::TCamera(fx32 yoffset, fx32 dist){
    mPersp.fovySin  = FX32_SIN30;
    mPersp.fovyCos  = FX32_COS30;
    mPersp.aspect   = FX32_ONE * 4 / 3;
    mPersp.nearClip = FX32_ONE;
    mPersp.farClip  = FX32_ONE * 400;

    mHeightOffset = yoffset;

    mLookAt.target.x	= 0;
	mLookAt.target.y	= mHeightOffset;
	mLookAt.target.z	= 0;

	mAngleX   = 256 * 20;
	mAngleY   = 0;
	mAngleZ   = 0;
	mDistance = dist;

    mMode = CAMERAMODE_FORWARD;
}

void TCamera::update(){
    if (mTarget != NULL){
        mLookAt.target.x = mTarget->getPosition().x;
        mLookAt.target.y = mTarget->getPosition().y + mHeightOffset;
        mLookAt.target.z = mTarget->getPosition().z;

        mLookAt.target.z += 8 * FX32_ONE * FX_CosIdx((u16)mTarget->getFacingDirection()) >> FX32_SHIFT;
        mLookAt.target.x += 8 * FX32_ONE * FX_SinIdx((u16)mTarget->getFacingDirection()) >> FX32_SHIFT;

        if (mMode == CAMERAMODE_FORWARD){
            mAngleY = (s16)(mTarget->getDirection() + 0x8000);
        }
        else{
            mAngleY = (s16)(mTarget->getDirection() + 0x0000);
        }
    }

    calc();
}

void TCamera::draw(){
    NNS_G3dGlbPerspective(mPersp.fovySin, mPersp.fovyCos, mPersp.aspect, mPersp.nearClip, mPersp.farClip);
    NNS_G3dGlbLookAt(&mLookAt.camPos, &mLookAt.camUp, &mLookAt.target);
}

void TCamera::calc(){
	fx16 sinx = FX_SinIdx((u16)mAngleX);
	fx16 cosx = FX_CosIdx((u16)mAngleX);
	fx16 siny = FX_SinIdx((u16)mAngleY);
	fx16 cosy = FX_CosIdx((u16)mAngleY);
	fx16 sinz = FX_SinIdx((u16)mAngleZ);
	fx16 cosz = FX_CosIdx((u16)mAngleZ);

	mLookAt.camPos.x = cosx * mDistance >> FX32_SHIFT;
	mLookAt.camPos.y = sinx * mDistance >> FX32_SHIFT;
	mLookAt.camPos.z = cosx * mDistance >> FX32_SHIFT;

	mLookAt.camPos.x = (mTarget->getPosition().x) + (mLookAt.camPos.x * siny >> FX32_SHIFT);
	mLookAt.camPos.y = (mTarget->getPosition().y) + (mLookAt.camPos.y);
	mLookAt.camPos.z = (mTarget->getPosition().z) + (mLookAt.camPos.z * cosy >> FX32_SHIFT);

    mLookAt.camUp.x =  FX_Mul( sinz, cosy );
    mLookAt.camUp.y =  cosz;
    mLookAt.camUp.z = -FX_Mul( sinz, siny );
}
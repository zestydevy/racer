#pragma once

#include <nitro.h>

#include "kart.hpp"

typedef struct
{
	VecFx32		camPos;					// camera position (= view point)
	VecFx32		target;					// camera target (= focus point)
	VecFx32		camUp;					// camera up direction
	MtxFx43		matrix;
} CamLookAt;

typedef struct
{
	fx32		fovySin;			// sine value of view angle divided by 2
    fx32		fovyCos;			// cosine value of view angle divided by 2
    fx32		aspect;				// aspect ratio
    fx32		nearClip;			// distance from view point to near clip surface
    fx32		farClip;			// distance from view point to far clip surface

} CamPersp;

enum CameraMode { CAMERAMODE_FORWARD, CAMERAMODE_REVERSE };

class TCamera
{
    public:
    TCamera(fx32 dist, fx32 yoffset);
    ~TCamera(){};

    void setTarget(TKart* target){mTarget = target;};
    void setAngle(s16 y){mAngleY = y;};
    void setDistance(fx32 dist){mDistance = dist;};
    void setMode(CameraMode mode){mMode = mode;};

    void update();
    void draw();

    protected:
    TKart* mTarget;
    CameraMode mMode;

    /* Camera state */
    CamPersp	mPersp;
	CamLookAt	mLookAt;
	fx32		mDistance;
	s16			mAngleX;
	s16			mAngleY;
	s16			mAngleZ;
    s16			padding;

    fx32 mHeightOffset;

    private:
    void calc();
};
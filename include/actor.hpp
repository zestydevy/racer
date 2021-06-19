#pragma once

#include <nitro.h>

#include "g3d_demolib.h"

class TActor
{
    public:
    TActor();
    ~TActor(){};

    void setPosition(VecFx32 & position){mPosition = position;}
    void setPosition(fx32 x, fx32 y, fx32 z){mPosition.x = x, mPosition.y = y, mPosition.z = z;}
    void setScale(VecFx32 & scale){mScale = scale;}
    void setScale(fx32 x, fx32 y, fx32 z){mScale.x = x, mScale.y = y, mScale.z = z;};

    void loadModel(char const * file);

    virtual void update();
    virtual void draw();

    protected:

    VecFx32 mPosition;
    VecFx32 mScale;

    NNSG3dRenderObj mRenderObj;
    NNSG3dResMdl * mModel;
    void * mModelFile;
};
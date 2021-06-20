#include "actor.hpp"

#include "g3d_demolib.h"

TActor::TActor()
{
    mPosition.x = 0;
    mPosition.y = 0;
    mPosition.z = 0;

    mScale.x = FX32_ONE;
    mScale.y = FX32_ONE;
    mScale.z = FX32_ONE;
}

void TActor::loadModel(char const * file)
{
    mModelFile = G3DDemo_LoadFile(file);
    
    if (!NNS_G3dResDefaultSetup(mModelFile)) {
        // can't initialize the model resource for some reason??
        return;
    }

    // unless the model file has more than one model, fetch model at index 0
    mModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(reinterpret_cast<NNSG3dResFileHeader *>(mModelFile)), 0);
    
    // create essentially the backbone of a model resource
    NNS_G3dRenderObjInit(&mRenderObj, mModel);
}

void TActor::update()
{
}

void TActor::draw()
{
    MtxFx33 identity;
    MTX_Identity33(&identity);

    NNS_G3dGlbSetBaseScale(&mScale);
    NNS_G3dGlbSetBaseTrans(&mPosition);
    NNS_G3dGlbSetBaseRot(&identity);
    NNS_G3dGlbFlushP();
    NNS_G3dDraw(&mRenderObj);
}
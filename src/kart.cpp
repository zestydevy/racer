#include "kart.hpp"

#include "g3d_demolib.h"

const fx32 GRAVITY = FX_F32_TO_FX32(1.0f);
const fx32 ACCEL = FX_F32_TO_FX32(0.1f);

void TKart::update()
{
    if (mGamepad->CheckHeld(PAD_BUTTON_A)){
        mPosition.z += FX_F32_TO_FX32(1.0f);
    }
}

void TKart::draw()
{
    NNS_G3dGlbSetBaseScale(&mScale);
    NNS_G3dGlbSetBaseTrans(&mPosition);
    NNS_G3dGlbFlushP();
    NNS_G3dDraw(&mRenderObj);
}
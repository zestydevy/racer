#include "game.hpp"
#include "actor.hpp"

#include "g3d_demolib.h"

G3DDemoCamera sCamera;

static TActor * sPlayer;
static TActor * sCourse;
static TActor * sSky;

void TGame::init()
{
    OS_Printf("Initializing game...\n");

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    NNS_G3dInit();

    G3DDemo_InitDisplay();

    // makes the manager able to manage 4 slots of texture image slots,
    // and sets to the default manager.
    NNS_GfdInitFrmTexVramManager(4, TRUE);

    // makes the manager able to manage 32 KB of palettes,
    // and sets to the default manager.
    NNS_GfdInitFrmPlttVramManager(0x8000, TRUE);

    sPlayer = new TActor;
    sCourse = new TActor;
    sSky = new TActor;

    sSky->setScale(FX32_ONE*200, FX32_ONE*200, FX32_ONE*200);
    sCourse->setScale(FX32_ONE*400, FX32_ONE*400, FX32_ONE*400);
    
    sPlayer->loadModel("data/yoshi.nsbmd");
    sCourse->loadModel("data/track.nsbmd");
    sSky->loadModel("data/sky.nsbmd");

    G3DDemo_InitCamera(&sCamera, 10*FX32_ONE, 16*FX32_ONE);

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "RACER");
}

void TGame::run()
{
    OS_Printf("Running game...\n");

    while(true)
    {
        SVC_WaitVBlankIntr();

        draw();
    }
}

void TGame::draw()
{
    G3DDemo_PrintApplyToHW();
    G3DDemo_ReadGamePad();
    
    G3DDemo_MoveCamera(&sCamera);
    G3DDemo_CalcCamera(&sCamera);

    G3X_Reset();
    G3X_ResetMtxStack();

    setCamera(&sCamera);

    // draw actors
    sPlayer->draw();
    sCourse->draw();
    sSky->draw();

    // restore translation and scale
    VecFx32 trans = {0,0,0};
    VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
    NNS_G3dGlbSetBaseScale(&scale);
    NNS_G3dGlbSetBaseTrans(&trans);

    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}

void TGame::setCamera(G3DDemoCamera * camera)
{
    G3DDemoLookAt* lookat = &camera->lookat;
    G3DDemoPersp*  persp  = &camera->persp;

    NNS_G3dGlbPerspective(persp->fovySin, persp->fovyCos, persp->aspect, persp->nearClip, persp->farClip);
    NNS_G3dGlbLookAt(&lookat->camPos, &lookat->camUp, &lookat->target);
}
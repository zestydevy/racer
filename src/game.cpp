#include "game.hpp"
#include "actor.hpp"
#include "array.hpp"
#include "kart.hpp"
#include "gamepad.hpp"
#include "camera.hpp"

#include "g3d_demolib.h"

static TCamera * sCamera;

static TKart * sPlayer;
static TActor * sCourse;
static TActor * sSky;

static TArray<TActor *> sActorList;
static TGamepad * sGamepad;

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

    sCamera = new TCamera(24*FX32_ONE, 32*FX32_ONE);
    sGamepad = new TGamepad;
    sPlayer = new TKart(sGamepad);
    sCourse = new TActor;
    sSky = new TActor;

    sActorList.push(sPlayer);
    sActorList.push(sCourse);
    sActorList.push(sSky);

    sCamera->setTarget(sPlayer);

    sSky->setScale(FX32_ONE*200, FX32_ONE*200, FX32_ONE*200);
    sCourse->setScale(FX32_ONE*400, FX32_ONE*400, FX32_ONE*400);
    
    sPlayer->loadModel("data/yoshi.nsbmd");
    sCourse->loadModel("data/track.nsbmd");
    sSky->loadModel("data/sky.nsbmd");

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

    G3X_Reset();
    G3X_ResetMtxStack();

    // camera
    sCamera->update();
    sCamera->draw();

    // update input
    sGamepad->readGamePad();

    // update actors
    sPlayer->update();

    // draw actors
    for (u32 i = 0; i < sActorList.size(); ++i) {
        sActorList[i]->draw();
    }

    // restore translation and scale
    VecFx32 trans = {0,0,0};
    VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
    MtxFx33 identity;
    MTX_Identity33(&identity);

    NNS_G3dGlbSetBaseScale(&scale);
    NNS_G3dGlbSetBaseTrans(&trans);
    NNS_G3dGlbSetBaseRot(&identity);

    NNS_G3dGlbFlushP();
    NNS_G3dGeFlushBuffer();

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}
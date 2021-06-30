#include <nnsys/g2d.h>

#include "game.hpp"
#include "actor.hpp"
#include "array.hpp"
#include "kart.hpp"
#include "gamepad.hpp"
#include "camera.hpp"
#include "file.hpp"
#include "gfx.hpp"

#include "g3d_demolib.h"

static TCamera * sCamera;

static TKart * sPlayer;
static TActor * sCourse;
static TActor * sSky;

static TArray<TActor *> sActorList;
static TGamepad * sGamepad;

NNSG2dPaletteData * pPltData;
NNSG2dCharacterData * pChrData;
NNSG2dScreenData * pScnData;

static NNSG2dImageProxy         sImageProxy;    // Character/texture proxy for cell
static NNSG2dImagePaletteProxy  sPaletteProxy;  // Palette proxy for cell

NNSG2dCellDataBank * sCellBank;

static void initDisplay()
{
    G3X_InitMtxStack();                                    // initialize the matrix stack
    GX_SetBankForTex(GX_VRAM_TEX_0123_ABCD);               // VRAM-ABCD for texture images
    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);          // VRAM-E for texture palettes
    GX_SetBankForBG(GX_VRAM_BG_16_G);                      // VRAM-C for BGs

	GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,               // graphics mode
	                   GX_BGMODE_0,                        // BGMODE is 4
	                   GX_BG0_AS_3D);                      // BG #0 is for 3D

    GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 );

	GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);
	GX_SetBGScrOffset ( GX_BGSCROFFSET_0x00000);

    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x3800,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01);

	G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);

	G3X_SetShading(GX_SHADING_TOON);                       // shading mode is toon
    G3X_AntiAlias(TRUE);                                   // enable antialias(without additional computing costs)
    G2_BlendNone();                                        // no 2D alpha blending or brightness change

    //---------------------------------------------------------------------------
    // After you start the geometry engine and the rendering one
    // (by GX_SetPower etc.), G3_SwapBuffers must be called once before use
    //---------------------------------------------------------------------------
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);


	// settings of light 0
//	G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);
//	G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

	G3X_AlphaTest(FALSE, 0);                   // AlphaTest OFF
	G3X_AlphaBlend(TRUE);                      // AlphaTest ON

    G3_ViewPort(0, 0, 255, 191);

	GX_DispOn();
	GXS_DispOn();
}

static NNSG2dCellDataBank * loadTestSprite()
{
    
    u8 * charFile = NULL;
    // do not free
    u8 * cellData = NULL;
    u8 * colorFile = NULL;
    NNSG2dCellDataBank * cellBank = NULL;
    NNSG2dCharacterData * charData = NULL;
    NNSG2dPaletteData * colorData = NULL;

    NNS_G2dInitImageProxy(&sImageProxy);
    NNS_G2dInitImagePaletteProxy(&sPaletteProxy);

    cellData = reinterpret_cast<u8 *>(TGfx::loadCellFile("OamSoftwareSpriteDraw.ncer", &cellBank));
    charFile = reinterpret_cast<u8 *>(TGfx::loadTileFile("OamSoftwareSpriteDraw.ncbr", &charData));
    colorFile = reinterpret_cast<u8 *>(TGfx::loadColorFile("OamSoftwareSpriteDraw.nclr", &colorData));

    // Loading For 2D Graphics Engine.
    NNS_G2dLoadImage2DMapping(
        charData,
        0x10000,
        NNS_G2D_VRAM_TYPE_3DMAIN,
        &sImageProxy
    );

    // Loading For 2D Graphics Engine.
    NNS_G2dLoadPalette(
        colorData,
        0,
        NNS_G2D_VRAM_TYPE_3DMAIN,
        &sPaletteProxy
    );

    return cellBank;
}

void TGame::init()
{
    OS_Printf("Initializing game...\n");

    NNS_G3dInit();

    initDisplay();

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

    /*
    // TODO: please free me....
    (void)TGfx::loadTileFile("test.NCGR", &pChrData);
    (void)TGfx::loadColorFile("test.NCLR", &pPltData);
    (void)TGfx::loadMapFile("test.NSCR", &pScnData);
    */

    sCellBank = loadTestSprite();

    GX_DispOn();
    GXS_DispOn();
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
    //G3X_ResetMtxStack(); //Called by G3X_Reset

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

    // for software sprite-setting
    {
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),        // diffuse
                                GX_RGB(16, 16, 16),        // ambient
                                TRUE                       // use diffuse as vtx color if TRUE
                                );

        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),        // specular
                                GX_RGB(0, 0, 0),           // emission
                                FALSE                      // use shininess table if TRUE
                                );

        G3_PolygonAttr(GX_LIGHTMASK_NONE,                  // no lights
                       GX_POLYGONMODE_MODULATE,            // modulation mode
                       GX_CULL_NONE,                       // cull back
                       0,                                  // polygon ID(0 - 63)
                       31,                                 // alpha(0 - 31)
                       0                                   // OR of GXPolygonAttrMisc's value
                       );
    }
    
    // init 2D camera
    {
        // Equivalent to unit matrix
        VecFx32 Eye = { 0, 0, 0 };                  // Eye position
        VecFx32 vUp = { 0, FX32_ONE, 0 };           // Up
        VecFx32 at = { 0, 0, -FX32_ONE };           // Viewpoint

        // Matrix mode is changed to GX_MTXMODE_POSITION_VECTOR internally,
        // and the camera matrix is loaded to the current matrix.
        G3_LookAt(&Eye, &vUp, &at, NULL);
    }
    
    {
        GXOamAttr tempOam;
        // Draws the number 0 OBJ.
        NNS_G2dCopyCellAsOamAttr(NNS_G2dGetCellDataByIdx(sCellBank, 0), 0, &tempOam );
        NNS_G2dSetupSoftwareSpriteCamera();

        NNS_G2dDrawOneOam3DDirectWithPos( 50, 20, 0, &tempOam, 
            &sImageProxy.attr,
            NNS_G2dGetImageLocation(&sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN),
            NNS_G2dGetImagePaletteLocation(&sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN)
        );
    }

    /*
    NNS_G2dBGSetup(
        NNS_G2D_BGSELECT_SUB2,
        pScnData,
        pChrData,
        pPltData,
        GX_BG_SCRBASE_0x0000,
        GX_BG_CHARBASE_0x08000
        );
    */

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}
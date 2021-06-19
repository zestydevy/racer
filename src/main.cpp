
#include <nitro.h>

#include "game.hpp"

static void bootGame()
{
    TGame game;

    game.init();
    game.run();
    game.exit();
}

extern "C" void NitroMain(void)
{
    void   *heapStart;
    void   *nstart;
    OSHeapHandle handle;

    OS_Init();
    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 2);
    OS_SetMainArenaLo(nstart);

    //---- Allocate region for heap from arena
    heapStart = OS_AllocFromMainArenaLo(0x4000, 32);

    //---- Create heap
    handle = OS_CreateHeap(OS_ARENA_MAIN, heapStart, (void *)((u32)heapStart + 0x4000));

    //---- Set current heap
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle);
    
    bootGame();
}
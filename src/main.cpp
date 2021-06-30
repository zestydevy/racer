
#include <nitro.h>

#include "game.hpp"

void * operator new(u32 size)
{
    return NNS_FndAllocFromExpHeapEx(G3DDemo_AppHeap, size, 32);
}
 
void operator delete(void * p)
{
    NNS_FndFreeToExpHeap(G3DDemo_AppHeap, p);    
}

void * operator new[](u32 size)
{
    return NNS_FndAllocFromExpHeapEx(G3DDemo_AppHeap, size, 32);
}
 
void operator delete[](void * p)
{
    NNS_FndFreeToExpHeap(G3DDemo_AppHeap, p);    
}

static void bootGame()
{
    TGame game;

    game.init();
    game.run();
    game.exit();
}

extern "C" void NitroMain(void)
{  
    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();
    
    bootGame();
}
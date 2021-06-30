#pragma once

#include <nitro.h>
#include <nnsys/g2d.h>

class TGfx
{
    public:
    static void * loadCellFile(char * const file, NNSG2dCellDataBank** cellBank);
    static void * loadTileFile(char * const file, NNSG2dCharacterData** tileBank);
    static void * loadColorFile(char * const file, NNSG2dPaletteData** colorBank);
    static void * loadMapFile(char * const file, NNSG2dScreenData** mapBank);
};
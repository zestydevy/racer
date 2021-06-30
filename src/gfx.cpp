#include "gfx.hpp"
#include "file.hpp"

void * TGfx::loadCellFile(char * const file, NNSG2dCellDataBank** cellBank)
{
    void * cellData = TFile::loadFile(file);

    if (cellData != NULL) {
        if(NNS_G2dGetUnpackedCellBank(cellData, cellBank)) {
            NNS_G2dPrintCellBank(*cellBank);
            return cellData;
        }
        delete reinterpret_cast<u8 *>(cellData);
    }
    OS_Warning("cannot load cell %s ...", file);
    return NULL;
}

void * TGfx::loadTileFile(char * const file, NNSG2dCharacterData** tileBank)
{
    void * tileData = TFile::loadFile(file);

    if (tileData != NULL) {
        if(NNS_G2dGetUnpackedCharacterData(tileData, tileBank)) {
            NNS_G2dPrintCharacterData(*tileBank);
            return tileData;
        }
        delete reinterpret_cast<u8 *>(tileData);
    }
    OS_Warning("cannot load tile %s ...", file);
    return NULL;
}

void * TGfx::loadColorFile(char * const file, NNSG2dPaletteData** colorBank)
{
    void * colorData = TFile::loadFile(file);

    if (colorData != NULL) {
        if(NNS_G2dGetUnpackedPaletteData(colorData, colorBank)) {
            NNS_G2dPrintPaletteData(*colorBank);
            return colorData;
        }
        delete reinterpret_cast<u8 *>(colorData);
    }
    OS_Warning("cannot load palette %s ...", file);
    return NULL;
}

void * TGfx::loadMapFile(char * const file, NNSG2dScreenData** mapBank)
{
    void * mapData = TFile::loadFile(file);

    if (mapData != NULL) {
        if(NNS_G2dGetUnpackedScreenData(mapData, mapBank)) {
            NNS_G2dPrintScreenData(*mapBank);
            return mapData;
        }
        delete reinterpret_cast<u8 *>(mapData);
    }
    OS_Warning("cannot load map %s ...", file);
    return NULL;
}
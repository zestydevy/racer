#include <string.h>

#include "file.hpp"

char const * sFilePrefix = "data/";

void * TFile::loadFile(char * const file)
{
    char * const nameBuff = new char[strlen(file) + strlen(sFilePrefix) + 1];
    u8 * fileData = NULL;
    u32 fileSize = 0;
    FSFile fsFile;

    (void)strcpy(nameBuff, sFilePrefix);
    (void)strcat(nameBuff, file);

    FS_InitFile(&fsFile);

    if (FS_OpenFile(&fsFile, nameBuff)) {

        fileSize = FS_GetLength(&fsFile);
        fileData = new u8[fileSize];
        
        (void)FS_ReadFile(&fsFile, reinterpret_cast<void *>(fileData), static_cast<s32>(fileSize));
        (void)FS_CloseFile(&fsFile);

        delete nameBuff;

        return reinterpret_cast<void *>(fileData);

    } else {
        OS_Printf("buddy you sure the file you're lookin for is there?\n");
        OS_Warning("can't find file %s ... panic\n", nameBuff);
        return NULL;
    }
}
#pragma once

#include <nitro.h>

#include "g3d_demolib.h"

class TGame
{
    public:

    TGame(){};
    ~TGame(){};

    void init();
    void run();
    void draw();
    void exit(){};

    void setCamera(G3DDemoCamera * camera);
};
#include "math.hpp"

fx32 TMath::bcerp(
    VecFx32 const & v1, 
    VecFx32 const & v2, 
    VecFx32 const & v3, 
    fx32 const worldX,
    fx32 const worldZ
)
{
    fx32 d = FX_MUL((v2.z - v3.z), (v1.x - v3.x)) + FX_MUL((v3.x - v2.x), (v1.z - v3.z));
    fx32 a = (FX_MUL((v2.z - v3.z), (worldX - v3.x)) + FX_MUL((v3.x - v2.x), (worldZ - v3.z))) / d;
    fx32 b = (FX_MUL((v3.z - v1.z), (worldX - v3.x)) + FX_MUL((v1.x - v3.x), (worldZ - v3.z))) / d;
    fx32 c = FX_F32_TO_FX32(1.0f) - a - b;
    return FX_MUL(a, v1.y) + FX_MUL(b, v2.y) + FX_MUL(c, v3.y);
}

#pragma once

#include <nitro.h>

/*
template<typename T>
class TMath {

  public:

  static const T epsilon() { return (T)1; }
  static const T zero() { return (T)0; }
  static const T one() { return (T)1; }

  static T abs(T x) { return (x >= zero() ? x : -x); }

  static inline T sqrt(T x) { return x; } // TODO integer sqrt

  static inline T mod(T x, T y) { return (x % y); }

  static inline T min(T x, T y) { return (x < y ? x : y); }
  static inline T max(T x, T y) { return (x > y ? x : y); }

  static inline T clamp(T x, T a, T b) {
    return min(max(x, a), b);
  }

};
*/

class TMath
{
    public:

    static fx32 bcerp(
      VecFx32 const & v1, 
      VecFx32 const & v2, 
      VecFx32 const & v3, 
      fx32 const worldX,
      fx32 const worldZ
    );
};
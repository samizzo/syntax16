#ifndef _vector_h
#define _vector_h

#include "types.h"

typedef struct Vector3_tag
{
    float x, y, z;
} Vector3;


#define vec3_set(dest, xx, yy, zz) dest.x = xx; dest.y = yy; dest.z = zz;
#define vec3_copy(dest, src) dest.x = src.x; dest.y = src.y; dest.z = src.z;
#define vec3_lerp(dest, a, b, t) dest.x = lerpf(a.x, b.x, t); dest.y = lerpf(a.y, b.y, t); dest.z = lerpf(a.z, b.z, t);

#endif

#ifndef _vector_h
#define _vector_h

#include "types.h"
#include <math.h>

typedef struct Vector3_tag
{
    float x, y, z;
} Vector3;


#define vec3_set(dest, xx, yy, zz) (dest).x = xx; (dest).y = yy; (dest).z = zz
#define vec3_copy(dest, src) (dest).x = (src).x; (dest).y = (src).y; (dest).z = (src).z
#define vec3_lerp(dest, a, b, t) dest.x = lerpf(a.x, b.x, t); dest.y = lerpf(a.y, b.y, t); dest.z = lerpf(a.z, b.z, t)
#define vec3_dist(a, b) sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z))
#define vec3_sub(dest, a, b) (dest).x = (a).x - (b).x; (dest).y = (a).y - (b).y; (dest).z = (a).z - (b).z
#define vec3_mul(dest, s) (dest).x *= s; (dest).y *= s; (dest).z *= s
#define vec3_add(dest, a, b) (dest).x = (a).x + (b).x; (dest).y = (a).y + (b).y; (dest).z = (a).z + (b).z

inline float vec3_length(Vector3* v)
{
    float len = sqrt((v->x*v->x) + (v->y*v->y) + (v->z*v->z));
    return len;
}

inline void vec3_normalise(Vector3* v)
{
    float ool = 1.0f / vec3_length(v);
    vec3_mul(*v, ool);
}

#endif

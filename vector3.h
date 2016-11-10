#ifndef _vector3_h
#define _vector3_h

#include "util.h"
#include <math.h>

typedef struct Vector3_tag
{
    float x, y, z;
} Vector3;


#define vec3_lerp(dest, a, b, t) dest.x = lerpf(a.x, b.x, t); dest.y = lerpf(a.y, b.y, t); dest.z = lerpf(a.z, b.z, t)
#define vec3_dist(a, b) sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z))

inline void vec3_add(Vector3* v, Vector3* a, Vector3* b)
{
    v->x = a->x + b->x;
    v->y = a->y + b->y;
    v->z = a->z + b->z;
}

inline void vec3_copy(Vector3* v, Vector3* src)
{
    v->x = src->x;
    v->y = src->y;
    v->z = src->z;
}

inline void vec3_mul(Vector3* v, float s)
{
    v->x *= s;
    v->y *= s;
    v->z *= s;
}

inline void vec3_sub(Vector3* v, Vector3* a, Vector3* b)
{
	v->x = a->x - b->x;
	v->y = a->y - b->y;
	v->z = a->z - b->z;
}

inline void vec3_set(Vector3* v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

inline float vec3_length(Vector3* v)
{
    float len = sqrt((v->x*v->x) + (v->y*v->y) + (v->z*v->z));
    return len;
}

inline void vec3_normalise(Vector3* v)
{
    float ool = 1.0f / vec3_length(v);
    vec3_mul(v, ool);
}

#endif

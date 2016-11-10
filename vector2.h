#ifndef _vector2_h
#define _vector2_h

#include "util.h"
#include <math.h>

typedef struct Vector2_tag
{
    float x, y;
} Vector2;


#define vec2_lerp(dest, a, b, t) dest.x = lerpf(a.x, b.x, t); dest.y = lerpf(a.y, b.y, t)
#define vec2_dist(a, b) sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y))

inline void vec2_add(Vector2* v, Vector2* a, Vector2* b)
{
    v->x = a->x + b->x;
    v->y = a->y + b->y;
}

inline void vec2_copy(Vector2* v, Vector2* src)
{
    v->x = src->x;
    v->y = src->y;
}

inline void vec2_mul(Vector2* v, float s)
{
    v->x *= s;
    v->y *= s;
}

inline void vec2_sub(Vector2* v, Vector2* a, Vector2* b)
{
	v->x = a->x - b->x;
	v->y = a->y - b->y;
}

inline void vec2_set(Vector2* v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
}

inline float vec2_length(Vector2* v)
{
    float len = sqrt((v->x*v->x) + (v->y*v->y));
    return len;
}

inline void vec2_normalise(Vector2* v)
{
    float ool = 1.0f / vec2_length(v);
    vec2_mul(v, ool);
}

#endif

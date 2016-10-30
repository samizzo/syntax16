#ifndef _vector_h
#define _vector_h

struct Vector3
{
    float x, y, z;
};


#define vec3_init(dest, xx, yy, zz) dest.x = xx; dest.y = yy; dest.z = zz;
#define vec3_copy(dest, src) dest.x = src.x; dest.y = src.y; dest.z = src.z;

#endif

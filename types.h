#ifndef _types_h
#define _types_h

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long int DWORD;

#define PI 3.14159265358979f

inline float lerpf(float a, float b, float t)
{
    return (a * t) + ((1 - t) * b);
}

#endif

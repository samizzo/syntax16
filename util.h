#ifndef _types_h
#define _types_h

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long int DWORD;

#define PI 3.14159265358979f

inline float lerpf(float a, float b, float t)
{
    return (b * t) + ((1 - t) * a);
}

inline float clampf(float value, float min, float max)
{
    return value > max ? max : value < min ? min : value;
}

extern DWORD g_seed;

// Returns a random value between 0 and 1.
inline DWORD random()
{
    g_seed = (214013 * g_seed) + 2531011;
    return (g_seed >> 16) & 0x7fff;
}

inline float randomf()
{
    return random() / (float)0x7fff;
}

#endif

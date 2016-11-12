#ifndef _util_h
#define _util_h

#include "types.h"

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

inline DWORD random()
{
    g_seed = (214013 * g_seed) + 2531011;
    return (g_seed >> 16) & 0x7fff;
}

// Returns a random value between 0 and 1.
inline float randomf()
{
    return random() / (float)0x7fff;
}

// Copy src to dst. Size is in dwords.
void util_blit(void* src, void* dst, DWORD size);

// Clear dest to col (8 pixels at once). Size is in dwords.
void util_clear(void *dest, DWORD col, DWORD size);

void timer_init();
long double timer_get_time();

#endif

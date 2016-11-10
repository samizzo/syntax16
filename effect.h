#ifndef _effect_h
#define _effect_h

typedef int (*Effect_InitFunc)();
typedef void (*Effect_UpdateFunc)(float);
typedef void (*Effect_StartFunc)();

typedef struct EffectDesc_tag
{
    Effect_InitFunc init;
    Effect_UpdateFunc update;
    Effect_StartFunc start;

    char started;
    char clearBuffer;
} EffectDesc;

#endif

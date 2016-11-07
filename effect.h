#ifndef _effect_h
#define _effect_h

typedef void (*Effect_InitFunc)();
typedef void (*Effect_UpdateFunc)();
typedef void (*Effect_StartFunc)();

typedef struct EffectDesc_tag
{
    Effect_InitFunc init;
    Effect_UpdateFunc update;
    Effect_StartFunc start;

    char started;
} EffectDesc;

#endif

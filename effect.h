#ifndef _effect_h
#define _effect_h

namespace Effect
{
    typedef void (*InitFunc)();
    typedef void (*UpdateFunc)();
    typedef void (*StartFunc)();

    struct EffectDesc
    {
        InitFunc init;
        UpdateFunc update;
        StartFunc start;

        bool started;
    };
}

#endif
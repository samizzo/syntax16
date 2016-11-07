#include "video.h"
#include "kb.h"
#include "polytun.h"
#include "dots.h"
#include "array.h"
#include <stdio.h>
#include "image.h"

Array* g_effects;

void registerEffects()
{
    array_add(g_effects, dots_getEffectDesc());
    array_add(g_effects, polytun_getEffectDesc());
}

void initEffects()
{
    int i = 0;
    for (i = 0; i < g_effects->size; i++)
    {
        EffectDesc* e = (EffectDesc*)array_get(g_effects, i);
        e->init();
        e->started = 0;
    }
}

static void init()
{
    g_effects = array_create(32, sizeof(EffectDesc));
}

static void cleanup()
{
    array_destroy(g_effects);
}

int main()
{
    Image* image;

    init();
    image = image_loadFromTGA("data/dot.tga");

    // registerEffects();
    // video::init();
    // initEffects();

    // Effect::EffectDesc* currentEffect = g_effects[0];

    // BYTE* buffer = video::getOffscreenBuffer();
    // DWORD bufferSize = (320 * 200) >> 2;

    // while (!kb::keyDown(kb::Escape))
    // {
    //     if (!currentEffect->started)
    //     {
    //         currentEffect->started = true;
    //         currentEffect->start();
    //     }

    //     currentEffect->update();

    //     video::waitForRetrace();
    //     video::flip(buffer, (void*)0xa0000, bufferSize);
    //     video::clear(buffer, 0, bufferSize);
    // }

    // video::deinit();

    cleanup();
    return 0;
}

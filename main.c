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
    registerEffects();
    video_init();
    initEffects();
}

static void cleanup()
{
    array_destroy(g_effects);
    video_deinit();
}

int main()
{
    BYTE* buffer;
    DWORD bufferSize = (320 * 200) >> 2;
    EffectDesc* currentEffect;

    init();

    currentEffect = (EffectDesc*)array_get(g_effects, 0);

    buffer = video_getOffscreenBuffer();

    while (!kb_keyDown(Key_Escape))
    {
        if (!currentEffect->started)
        {
            currentEffect->started = 1;
            currentEffect->start();
        }

        currentEffect->update();

        video_waitForRetrace();
        video_flip(buffer, (void*)0xa0000, bufferSize);
        video_clear(buffer, 0, bufferSize);
    }

    cleanup();
    return 0;
}

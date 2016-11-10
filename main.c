#include "video.h"
#include "kb.h"
#include "polytun.h"
#include "dots.h"
#include "array.h"
#include "image.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

Array* g_effects;

void registerEffects()
{
    array_add(g_effects, polytun_getEffectDesc());
    array_add(g_effects, dots_getEffectDesc());
}

int initEffects()
{
    int success = 1;
    int i = 0;
    for (i = 0; i < g_effects->size; i++)
    {
        EffectDesc* e = (EffectDesc*)array_get(g_effects, i);
        success &= e->init();
        e->started = 0;
    }
    return success;
}

static void init()
{
    g_effects = array_create(32, sizeof(EffectDesc));
    registerEffects();
    if (!initEffects())
    {
        log_debug("Failed to init effects");
        exit(0);
    }

    video_init();
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
    video_clear(buffer, 0, bufferSize);

    while (!kb_keyDown(Key_Escape))
    {
        static const float dt = 1.0f / 60.0f;

        if (!currentEffect->started)
        {
            currentEffect->started = 1;
            currentEffect->start();
        }

        currentEffect->update(dt);

        video_waitForRetrace();
        video_flip(buffer, (void*)0xa0000, bufferSize);
        if (currentEffect->clearBuffer)
            video_clear(buffer, 0, bufferSize);
    }

    cleanup();
    return 0;
}

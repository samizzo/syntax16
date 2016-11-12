#include <stdio.h>
#include <stdlib.h>
#include "video.h"
#include "kb.h"
#include "array.h"
#include "image.h"
#include "log.h"
#include "control.h"

// Effects
#include "polytun.h"
#include "dots.h"
#include "plasma.h"

Array* g_effects;

void registerEffects()
{
    array_add(g_effects, plasma_getEffectDesc());
    array_add(g_effects, dots_getEffectDesc());
    array_add(g_effects, polytun_getEffectDesc());
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

void init_fpu(void)
{
    unsigned short control_word;

    __asm
    {
        finit
        fstcw [control_word]
        mov bx,[control_word]
        and bh,0fch  // 24bits precision, no overflow, no divide by zero exceptions
        mov [control_word],bx
        fldcw [control_word]
    };
}

static void init()
{
    log_init("debug.txt");
    log_debug("log file opened\n");

    init_fpu();
    TMRinit(PENTIUM_TIMER);

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
    log_shutdown();
    array_destroy(g_effects);
    video_deinit();
    TMRremove();
}

int main()
{
    BYTE* buffer;
    DWORD bufferSize = (320 * 200) >> 2;
    EffectDesc* currentEffect;
    long double lastTime;

    init();

    currentEffect = (EffectDesc*)array_get(g_effects, 0);

    buffer = video_getOffscreenBuffer();
    util_clear(buffer, 0, bufferSize);

    lastTime = TMRgettime();

    while (!kb_keyDown(Key_Escape))
    {
        long double currentTime = TMRgettime();
        long double delta = currentTime - lastTime;
        lastTime = currentTime;

        if (!currentEffect->started)
        {
            currentEffect->started = 1;
            currentEffect->start();
        }

        currentEffect->update((float)SECONDS(delta));

        video_waitForRetrace();
        util_blit(buffer, (void*)0xa0000, bufferSize);
        if (currentEffect->clearBuffer)
            util_clear(buffer, 0, bufferSize);
    }

    cleanup();
    return 0;
}

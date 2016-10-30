#include "video.h"
#include "kb.h"
#include "polytun.h"
#include "array.h"
#include <stdio.h>

Array<Effect::EffectDesc*, 32> g_effects;

void registerEffects()
{
    g_effects.push_back(polytun::getEffectDesc());
}

void initEffects()
{
    for (int i = 0; i < g_effects.length(); i++)
    {
        Effect::EffectDesc* e = g_effects[i];
        e->init();
        e->started = false;
    }
}

int main()
{
    registerEffects();
    video::init();
    initEffects();

    Effect::EffectDesc* currentEffect = g_effects[0];

    BYTE* buffer = video::getOffscreenBuffer();
    DWORD bufferSize = (320 * 200) >> 2;

    while (!kb::keyDown(kb::Escape))
    {
        if (!currentEffect->started)
        {
            currentEffect->started = true;
            currentEffect->start();
        }

        currentEffect->update();

        video::waitForRetrace();
        video::flip(buffer, (void*)0xa0000, bufferSize);
        video::clear(buffer, 0, bufferSize);
    }

    video::deinit();
}

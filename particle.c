#include <stdlib.h>
#include "particle.h"
#include "video.h"
#include "image.h"

ParticleSystem* ps_create(int maxParticles)
{
    int i;
    ParticleSystem* ps = (ParticleSystem*)malloc(sizeof(ParticleSystem));
    ps->particles = malloc(sizeof(Particle) * maxParticles);
    ps->maxParticles = maxParticles;

    for (i = 0; i < maxParticles; i++)
    {
        Particle* p = &ps->particles[i];
        p->life = 0.0f;
    }

    return ps;
}

void ps_updateAndDraw(ParticleSystem* ps, BYTE* buffer, float dt)
{
    int x, y, xx, yy, i, width, height; //, prevx, prevy;
    Image* image;
    BYTE* pixels, *b;
    float life;
    const int maxParticles = ps->maxParticles;
    Particle* particles = ps->particles;

    if (!buffer)
        buffer = video_getOffscreenBuffer();

    for (i = 0; i < maxParticles; i++)
    {
        Vector2* p, *v, *a;
        Vector2 tmp;
        Particle* d = &particles[i];

        if (d->life <= 0.0f)
            continue;

        p = &d->position;
        v = &d->velocity;
        a = &d->acceleration;

        // prevx = (int)p->x;
        // prevy = (int)p->y;

        // Apply acceleration to velocity.
        vec2_copy(&tmp, a);
        vec2_mul(&tmp, dt);
        vec2_add(v, v, &tmp);

        // Apply velocity to position.
        vec2_copy(&tmp, v);
        vec2_mul(&tmp, dt);
        vec2_add(p, p, &tmp);

        life = d->life;

        // Update lifetime.
        d->life -= dt;

        image = d->image;
        width = image->width;
        height = image->height;

        // Draw it.
        x = (int)p->x - (width >> 1);
        y = (int)p->y - (height >> 1);

        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
            continue;

        pixels = image->pixels;
        b = buffer + x + (y * SCREEN_WIDTH);

        for (yy = 0; yy < height; yy++)
        {
            for (xx = 0; xx < width; xx++)
            {
                BYTE col = *pixels;
                BYTE c = *b;
                *b = (BYTE)min((col * life) + c, 255);
                b++;
                pixels++;
            }

            b += SCREEN_WIDTH - image->width;
        }

        // pixels = image->pixels;
        // b = buffer + prevx + (prevy * SCREEN_WIDTH);
        // for (yy = 0; yy < height; yy++)
        // {
        //     for (xx = 0; xx < width; xx++)
        //     {
        //         BYTE col = *pixels;
        //         BYTE c = *b;
        //         *b = (BYTE)min((col * life) + c, 255);
        //         b++;
        //         pixels++;
        //     }

        //     b += SCREEN_WIDTH - image->width;
        // }
    }
}

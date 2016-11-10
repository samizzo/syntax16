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
    int x, y, xx, yy, i, width, height;
    Image* image;
    BYTE* pixels, *b;
    float life;
    const int maxParticles = ps->maxParticles;
    Particle* particles = ps->particles;

    if (!buffer)
        buffer = video_getOffscreenBuffer();

    for (i = 0; i < maxParticles; i++)
    {
        Vector3* p, *v, *a;
        Vector3 tmp;
        Particle* d = &particles[i];

        if (d->life <= 0.0f)
            continue;

        p = &d->position;
        v = &d->velocity;
        a = &d->acceleration;

        // Apply acceleration to velocity.
        vec3_copy(&tmp, a);
        vec3_mul(&tmp, dt);
        vec3_add(v, v, &tmp);

        // Apply velocity to position.
        vec3_copy(&tmp, v);
        vec3_mul(&tmp, dt);
        vec3_add(p, p, &tmp);

        life = d->life;

        // Update lifetime.
        d->life -= dt;

        // Draw it.
        x = (int)p->x;
        y = (int)p->y;

        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
            continue;

        image = d->image;
        pixels = image->pixels;
        b = buffer + x + (y * SCREEN_WIDTH);
        width = image->width;
        height = image->height;

        for (yy = 0; yy < height; yy++)
        {
            for (xx = 0; xx < width; xx++)
            {
                BYTE col = *pixels;
                BYTE c = *b;
                *b = (BYTE)clamp((col * life) + c, 0, 255);
                b++;
                pixels++;
            }

            b += SCREEN_WIDTH - image->width;
        }
    }
}

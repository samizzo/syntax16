#include <math.h>
#include <memory.h>
#include "dots.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include "image.h"
#include "log.h"
#include "util.h"

typedef struct Particle_tag
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float life;
    Image* image;
} Particle;

#define MAX_PARTICLES 1024
static Particle s_particles[MAX_PARTICLES];
static const float RADIUS = 150.0f;
static Vector3 s_centre = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
static float s_maxAcceleration = 40.0f;

Image* images[3];

static BYTE s_dot[3*3] =
{
    0,   80, 0,
    70, 100, 100,
    0,   50, 0,
};

static BYTE s_dot2[3*3] =
{
    0,   70, 0,
    70, 120, 40,
    0,   20, 0,
};

static BYTE s_dot3[3*3] =
{
    0,   40, 0,
    60, 180, 40,
    0,   30, 0,
};

static void addParticles(int num)
{
    int i, j;

    // FIXME: Go through all particles once, rather than num times!

    for (i = 0; i < num; i++)
    {
        Particle* particle = 0;

        // Find a free particle slot.
        for (j = 0; j < MAX_PARTICLES; j++)
        {
            Particle* d = &s_particles[j];
            if (d->life <= 0.0f)
            {
                particle = d;
                break;
            }
        }

        // If we can't find any more free slots, just abort.
        if (!particle)
            break;

        {
            Vector3* p = &particle->position;
            Vector3* v = &particle->velocity;
            Vector3* a = &particle->acceleration;
            float t;

            vec3_set(v, 0, 0, 0);

            // Randomise position around circle.
            t = randomf();
            p->x = (cos(t * 2.0f * PI) * RADIUS * 0.5f);
            p->y = (sin(t * 2.0f * PI) * RADIUS * 0.5f);
            p->z = 0.0f;
            vec3_add(p, p, &s_centre);

            // Randomise acceleration.
            vec3_sub(a, &s_centre, p);
            vec3_normalise(a);
            vec3_mul(a, randomf() * s_maxAcceleration);

            particle->life = 1.5f;
            particle->image = images[random() & 2];
        }
    }
}

static int init()
{
    int i;

    for (i = 0; i < 3; i++)
        images[i] = image_create(3, 3);
    memcpy(images[0]->pixels, s_dot, 3*3);
    memcpy(images[1]->pixels, s_dot2, 3*3);
    memcpy(images[2]->pixels, s_dot3, 3*3);

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        Particle* d = &s_particles[i];
        d->life = 0.0f;
    }

    addParticles(50);

    // image = image_loadFromTGA("data/dot.tga");
    // if (!image)
    // {
    //     log_debug("Failed to load dot.tga");
    //     return 0;
    // }

    // image_remapPaletteLinear(image);

    return 1;
}

static void start()
{
    int i;
    Vector3 startCol, endCol;
    endCol.x = 181 * 64 / 256.0f;
    endCol.y = 219 * 64 / 256.0f;
    endCol.z = 182 * 64 / 256.0f;
    startCol.x = startCol.y = startCol.z = 0.0f;

    for (i = 0; i < 256; i++)
    {
        Vector3 col;
        vec3_lerp(col, startCol, endCol, i / (float)256);
        video_setPal((BYTE)i, (BYTE)col.x, (BYTE)col.y, (BYTE)col.z);
    }

    //video_setPalette(image->palette);
}

static void drawParticle(Particle* p)
{
    int x, y, xx, yy;
    BYTE* buffer;
    Image* image;
    BYTE* imageBuf;
    float life;

    x = (int)(p->position.x);
    y = (int)(p->position.y);

    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return;

    buffer = video_getOffscreenBuffer();
    image = p->image;
    imageBuf = image->pixels;
    life = p->life;

    buffer += x + (y * SCREEN_WIDTH);

    for (yy = 0; yy < image->height; yy++)
    {
        for (xx = 0; xx < image->width; xx++)
        {
            BYTE col = *imageBuf;
            BYTE c = *buffer;
            *buffer = (BYTE)clamp((col * life) + c, 0, 255);
            buffer++;
            imageBuf++;
        }

        buffer += SCREEN_WIDTH - image->width;
    }
}

static float s_timer;

static void update(float dt)
{
    int i;

    s_timer += dt;
    if (s_timer > 0.2f)
    {
        s_timer = 0.0f;
        addParticles(100);
    }

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        Particle* d = &s_particles[i];
        Vector3* p, *v, *a;
        Vector3 tmp;

        if (d->life <= 0.0f)
            continue;

        p = &d->position;
        v = &d->velocity;
        a = &d->acceleration;

        // Apply acceleration to velocity
        vec3_copy(&tmp, a);
        vec3_mul(&tmp, dt);
        vec3_add(v, v, &tmp);

        // Apply velocity to position
        vec3_copy(&tmp, v);
        vec3_mul(&tmp, dt);
        vec3_add(p, p, &tmp);

        drawParticle(d);
        d->life -= dt;
    }

    if (kb_keyDown(Key_A))
    {
        if (kb_keyDown(Key_LShift))
        {
            s_maxAcceleration -= 1;
        }
        else
        {
            s_maxAcceleration += 1;
        }
    }
}

static EffectDesc s_desc = { init, update, start };

EffectDesc* dots_getEffectDesc()
{
    return &s_desc;
}

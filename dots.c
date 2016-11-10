#include "dots.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include "image.h"
#include "log.h"
#include <math.h>
#include <memory.h>

typedef struct Particle_tag
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float life;
} Particle;

#define MAX_PARTICLES 128
static Particle s_particles[MAX_PARTICLES];

Image* image = 0;

static BYTE s_dot[4*4] =
{
    0, 0, 0, 0,
    0, 255, 240, 0,
    0, 231, 209, 0,
    0, 0, 0, 0
};

static int init()
{
    static const float RADIUS = 150.0f;
    Vector3 centre = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
    int i;

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        Particle* d = &s_particles[i];
        Vector3* p = &d->position;
        Vector3* v = &d->velocity;
        Vector3* a = &d->acceleration;

        float t = i / (float)(MAX_PARTICLES);
        p->x = (cos(t * 2.0f * PI) * RADIUS * 0.5f);
        p->y = (sin(t * 2.0f * PI) * RADIUS * 0.5f);
        p->z = 0.0f;
        vec3_add(*p, *p, centre);

        vec3_set(v, 0, 0, 0);
        vec3_sub(a, &centre, p);
        vec3_normalise(a);
        //vec3_mul(a, )

        d->life = 1.0f;
    }

    // image = image_loadFromTGA("data/dot.tga");
    // if (!image)
    // {
    //     log_debug("Failed to load dot.tga");
    //     return 0;
    // }

    // image_remapPaletteLinear(image);

    image = image_create(4, 4);
    memcpy(image->pixels, s_dot, 4*4);

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

static void drawDot(int x, int y, float life)
{
    int xx, yy;
    BYTE* buffer = video_getOffscreenBuffer();
    BYTE* imageBuf = image->pixels;

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

static void update()
{
    int i, x, y;
    BYTE* buffer = video_getOffscreenBuffer();

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        Particle* d = &s_particles[i];
        Vector3* p = &d->position;
        x = (int)(p->x);
        y = (int)(p->y);

        if (d->life > 0.0f)
        {
            Vector3 delta;
            Vector3* v = &d->velocity;
            vec3_copy(delta, *v);
            vec3_mul(&delta, 0.1f);
            vec3_add(*p, *p, delta);

            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
                drawDot(x, y, d->life);

            d->life -= 0.001f;
        }
    }
}

static EffectDesc s_desc = { init, update, start };

EffectDesc* dots_getEffectDesc()
{
    return &s_desc;
}

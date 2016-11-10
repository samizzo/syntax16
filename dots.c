#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "dots.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include "image.h"
#include "log.h"
#include "util.h"
#include "particle.h"

static const int MAX_PARTICLES = 4096;
static ParticleSystem* s_system;
static const float RADIUS = 150.0f;
static Vector3 s_centre = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
static float s_maxAcceleration = 40.0f;

#define DO_BLUR 0

#if DO_BLUR
static BYTE* s_blurBuffer;
#endif

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
    int j;
    int maxParticles = s_system->maxParticles;
    Particle* particles = s_system->particles;

    for (j = 0; j < maxParticles; j++)
    {
        Particle* d = particles + j;
        if (d->life <= 0.0f)
        {
            Vector3* p = &d->position;
            Vector3* v = &d->velocity;
            Vector3* a = &d->acceleration;
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

            d->life = 1.5f;
            d->image = images[random() & 2];

            num--;
            if (num == 0)
                break;
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

#if DO_BLUR
    s_blurBuffer = (BYTE*)malloc(SCREEN_WIDTH*SCREEN_HEIGHT);
    video_clear(s_blurBuffer, 0, (SCREEN_WIDTH*SCREEN_HEIGHT)>>2);
#endif

    s_system = ps_create(MAX_PARTICLES);
    addParticles(100);

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

static float s_timer;

#if DO_BLUR
static int s_blurAmount = 0;
static int s_blurAdder = 1;
#endif

#if DO_BLUR
static void blur()
{
    int x, y;
    BYTE* buffer = video_getOffscreenBuffer();

    if (s_blurAmount == 0)
        return;

    for (y = 10; y < SCREEN_HEIGHT - 10; y++)
    {
        for (x = s_blurAmount; x < SCREEN_WIDTH - s_blurAmount; x++)
        {
            int xx;
            int ofs = x + (y * SCREEN_WIDTH);
            BYTE* pixel = s_blurBuffer + ofs;
            BYTE* b = buffer + ofs - s_blurAmount;
            DWORD c = 0;

            for (xx = -s_blurAmount; xx < s_blurAmount; xx++)
            {
                c += *b;
                b++;
            }

            c /= (s_blurAmount << 1);
            *pixel = c;
        }
    }

    // FIXME: Just copy s_blurBuffer directly to the screen.
    video_flip(s_blurBuffer, buffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}
#endif

static void update(float dt)
{
    s_timer += dt;
    if (s_timer > 0.1f)
    {
        s_timer = 0.0f;
        addParticles(200);
    }

#if DO_BLUR
    s_blurAmount += s_blurAdder;
    if (s_blurAmount == 10)
    {
        s_blurAdder = -1;
    }
    else if (s_blurAmount == 0)
    {
        s_blurAdder = 1;
    }
#endif

    ps_updateAndDraw(s_system, video_getOffscreenBuffer(), dt);

#if DO_BLUR
    blur();

    if (kb_keyDown(Key_A))
    {
        if (kb_keyDown(Key_LShift))
        {
            s_blurAmount -= 1;
        }
        else
        {
            s_blurAmount += 1;
        }
    }
#endif
}

static EffectDesc s_desc = { init, update, start };

EffectDesc* dots_getEffectDesc()
{
    return &s_desc;
}

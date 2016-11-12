#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "dots.h"
#include "video.h"
#include "Vector2.h"
#include "Vector3.h"
#include "kb.h"
#include "image.h"
#include "log.h"
#include "util.h"
#include "particle.h"

static const int MAX_PARTICLES = 2048;
static ParticleSystem* s_system;
static const float RADIUS = 150.0f;
static Vector2 s_centre = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
static float s_maxAcceleration = 40.0f;

static float s_bigParticleChance = 0.2f; // Probability of spawning a big particle (smaller means more likely)

#define DO_BLUR 0

#if DO_BLUR
static BYTE* s_blurBuffer;
#endif

Image* images[4];
BYTE* palette;

BYTE dot[8*8] = {160, 160, 160, 162, 162, 160, 160, 160,
                 160, 165, 173, 178, 176, 172, 163, 160,
                 162, 175, 184, 190, 189, 184, 173, 160,
                 169, 183, 194, 200, 199, 191, 181, 166,
                 171, 185, 196, 204, 202, 194, 182, 168,
                 167, 181, 191, 198, 197, 190, 179, 166,
                 160, 173, 182, 187, 187, 181, 172, 160,
                 160, 160, 168, 172, 171, 167, 160, 160};

static BYTE s_dot[2*2] =
{
    0,   80,
    70, 255,
};

static BYTE s_dot2[2*2] =
{
    0,   70,
    70, 120,
};

static BYTE s_dot3[2*2] =
{
    0,   70,
    180, 0,
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
            Vector2* p = &d->position;
            Vector2* v = &d->velocity;
            Vector2* a = &d->acceleration;
            float t;

            vec2_set(v, 0, 0, 0);

            // Randomise position around circle.
            t = randomf();
            p->x = (cos(t * 2.0f * PI) * RADIUS * 0.5f);
            p->y = (sin(t * 2.0f * PI) * RADIUS * 0.5f);
            vec2_add(p, p, &s_centre);

            // Randomise acceleration.
            vec2_sub(a, &s_centre, p);
            vec2_normalise(a);
            vec2_mul(a, randomf() * s_maxAcceleration);

            d->life = 1.5f;

            t = randomf();
            if (t > s_bigParticleChance)
            {
                d->image = images[3];
            }
            else
            {
                d->image = images[random() & 2];
            }

            num--;
            if (num == 0)
                break;
        }
    }
}

static int init()
{
    int i;

    palette = image_loadPalette("data/pal1.pal");

    for (i = 0; i < 3; i++)
        images[i] = image_create(2, 2);
    memcpy(images[0]->pixels, s_dot, 2*2);
    memcpy(images[1]->pixels, s_dot2, 2*2);
    memcpy(images[2]->pixels, s_dot3, 2*2);
    images[3] = image_create(8, 8);
    for (i = 0; i < 8*8; i++) dot[i] -= 160;

    memcpy(images[3]->pixels, dot, 8*8);

#if DO_BLUR
    s_blurBuffer = (BYTE*)malloc(SCREEN_WIDTH*SCREEN_HEIGHT);
    video_clear(s_blurBuffer, 0, (SCREEN_WIDTH*SCREEN_HEIGHT)>>2);
#endif

    s_system = ps_create(MAX_PARTICLES);
    //addParticles(10);

    // image = image_loadFromTGA("data/dot.tga");
    // if (!image)
    // {
    //     log_debug("Failed to load dot.tga");
    //     return 0;
    // }

    //image_remapPaletteLinear(palette);

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

    video_setPalette(palette);
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
    util_blit(s_blurBuffer, buffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}
#endif

static void update(float dt)
{
    s_timer += dt;
    if (s_timer > 0.1f)
    {
        s_timer = 0.0f;
        addParticles(60);
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

    //video_drawPalette();

    ps_updateAndDraw(s_system, video_getOffscreenBuffer(), dt);

    {
        int x;
        BYTE* b = video_getOffscreenBuffer();
        for (x = 0; x < SCREEN_WIDTH * SCREEN_HEIGHT; x++)
        {
            int c = *b;
            c = max(0, c - 30);
            *b = c;
            b++;
        }
    }

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

static EffectDesc s_desc = { init, update, start, 0, 0 };

EffectDesc* dots_getEffectDesc()
{
    return &s_desc;
}

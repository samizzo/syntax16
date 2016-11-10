#include "dots.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include "image.h"
#include "log.h"
#include <math.h>

typedef struct Dot_tag
{
    Vector3 position;
    Vector3 velocity;
    float life;
} Dot;

#define MAX_DOTS 1024
static Dot s_dots[MAX_DOTS];

Image* image = 0;

static int init()
{
    #define RADIUS 150.0f
    int i;

    for (i = 0; i < MAX_DOTS; i++)
    {
        float t = i / (float)(MAX_DOTS >> 1);
        float x = (cos(t * 2.0f * PI) * RADIUS * 0.5f) + (SCREEN_WIDTH * 0.5f);
        float y = (sin(t * 2.0f * PI) * RADIUS * 0.5f) + (SCREEN_HEIGHT * 0.5f);
        vec3_set(s_dots[i].position, x, y, 0.0f);
        vec3_set(s_dots[i].velocity, 0.0f, 0.0f, 0.0f);
        s_dots[i].life = 1.0f;
    }

    image = image_loadFromTGA("data/dot.tga");
    if (!image)
    {
        log_debug("Failed to load dot.tga");
        return 0;
    }

    image_remapPaletteLinear(image);
    return 1;
}

static void start()
{
    int i;
    Vector3 startCol, endCol;
    startCol.x = 181 * 64 / 256.0f;
    startCol.y = 219 * 64 / 256.0f;
    startCol.z = 182 * 64 / 256.0f;
    endCol.x = endCol.y = endCol.z = 0.0f;

    for (i = 0; i < 256; i++)
    {
        Vector3 col;
        vec3_lerp(col, startCol, endCol, i / (float)256);
        video_setPal((BYTE)i, (BYTE)col.x, (BYTE)col.y, (BYTE)col.z);
    }

    //video_setPalette(image->palette);
}

static void drawDot(int x, int y)
{
    int xx, yy;
    BYTE* buffer = video_getOffscreenBuffer();
    BYTE* imageBuf = (BYTE*)image->pixels;

    x -= image->width >> 1;
    y -= image->height >> 1;

    buffer += x + (y * SCREEN_WIDTH);

    for (yy = 0; yy < image->height; yy++)
    {
        for (xx = 0; xx < image->width; xx++)
        {
            *buffer = *imageBuf;
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

    // for (x = 0; x < 256; x++)
    // {
    //     for (y = 0; y < 200; y++)
    //     {
    //         buffer[x + (y * 320)] = (BYTE)x;
    //     }
    // }

    for (i = 0; i < MAX_DOTS; i++)
    {
        x = (int)(s_dots[i].position.x);
        y = (int)(s_dots[i].position.y);
        if (s_dots[i].life > 0.0f)
        {
            //buffer[x + (y * 320)] = (BYTE)(255 * s_dots[i].life);
            //s_dots[i].life -= 0.001f;
            // drawDot(x, y);
        }
    }

    // {
    //     BYTE* buf = image->pixels;
    //     for (y = 0; y < image->height; y++)
    //     {
    //         for (x = 0; x < image->width; x++)
    //         {
    //             BYTE c = *buf;
    //             buffer[x + (y * 320)] = c;
    //             buf++;
    //         }
    //     }
    // }
}

static EffectDesc s_desc = { init, update, start };

EffectDesc* dots_getEffectDesc()
{
    return &s_desc;
}
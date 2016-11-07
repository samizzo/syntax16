#include "polytun.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include <math.h>
#include <stdio.h>

static const float X_CENTRE = SCREEN_WIDTH * 0.5f;
static const float Y_CENTRE = SCREEN_HEIGHT * 0.5f;
static const float ASPECT = (SCREEN_HEIGHT / (float)SCREEN_WIDTH) * (320.0f / 240.0f);
static float s_fovHori = 60.0f;

#define MAX_SEGMENTS 10
static Vector3 m_world[MAX_SEGMENTS * 4];
static Vector3 m_screen[MAX_SEGMENTS * 4];
static int s_numSegments = 0;
static float s_projXscale;
static float s_projYscale;

// Adds a segment to the end of the list with the specified z position.
static void addSegment(float z)
{
    int ofs = s_numSegments * 4;

    if (s_numSegments == MAX_SEGMENTS)
        return;

    vec3_set(m_world[ofs+0], -0.5f, -0.5f, z); // top left
    vec3_set(m_world[ofs+1], 0.5f, -0.5f, z); // top right
    vec3_set(m_world[ofs+2], 0.5f, 0.5f, z); // bottom right
    vec3_set(m_world[ofs+3], -0.5f, 0.5f, z); // bottom left

    s_numSegments++;
}

void projectTunnel()
{
    Vector3 s;
    Vector3 w;
    int i;

    for (i = 0; i < s_numSegments * 4; i++)
    {
        float z;
        vec3_copy(w, m_world[i]);

        z = 1.0f / w.z;
        s.x = X_CENTRE + (w.x * s_projXscale * z);
        s.y = Y_CENTRE + (w.y * s_projYscale * z);
        s.z = z;

        vec3_copy(m_screen[i], s);
    }
}

static void renderTunnel()
{
    Vector3 screen;
    int i;
    BYTE* buffer = video_getOffscreenBuffer();

    for (i = 0; i < s_numSegments; i++)
    {
        int x, y, j;
        BYTE colour = (BYTE)((m_screen[i].z * 127.0f) + (((i >> 2) & 1) << 7));
        vec3_copy(screen, m_screen[i]);

        x = (int)screen.x;
        y = (int)screen.y;
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
            buffer[x + (y * SCREEN_WIDTH)] = colour;

        for (j = 0; j < 4; j++)
        {
            
        }
    }
}

static void init()
{
    float z = 1.0f;
    int i;
    for (i = 0; i < MAX_SEGMENTS; i++)
    {
        addSegment(z);
        z += 1.0f;
    }
}

static void start()
{
    int i;
    for (i = 0; i < 128; i++)
    {
        video_setPal((BYTE)i, (BYTE)(i >> 2), (BYTE)(i >> 2), (BYTE)(i >> 1));
        video_setPal((BYTE)(i + 128), (BYTE)(i >> 1), (BYTE)(i >> 2), (BYTE)(i >> 8));
    }
}

float scale = 0.8333f;

static void update()
{
    s_projXscale = SCREEN_WIDTH / (2.0f * (float)tan(s_fovHori * 0.5f * PI / 180.0f));
    s_projYscale = s_projXscale * ASPECT;

    projectTunnel();
    renderTunnel();

    // BYTE* buffer = video_getOffscreenBuffer();
    // for (int x = 0; x < 256; x++)
    // {
    //     for (int y = 0; y < 200; y++)
    //     {
    //         buffer[x + (y * 320)] = (BYTE)x;
    //     }
    // }

    if (kb_keyDown(Key_Up))
    {
        s_fovHori += 0.1f;
        if (s_fovHori > 90.0f)
            s_fovHori = 90.0f;
    }
    else if (kb_keyDown(Key_Down))
    {
        s_fovHori -= 0.1f;
        if (s_fovHori < 10.0f)
            scale = 10.0f;
    }
}

static EffectDesc s_desc = { init, update, start };

EffectDesc* polytun_getEffectDesc()
{
    return &s_desc;
}

#include <math.h>
#include <stdio.h>
#include "log.h"
#include "polytun.h"
#include "video.h"
#include "vector3.h"
#include "kb.h"
#include "poly.h"

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
static Vector3 s_camera;

// Adds a segment to the end of the list with the specified z position.
static void addSegment(float z)
{
    int ofs = s_numSegments * 4;

    if (s_numSegments == MAX_SEGMENTS)
        return;

    vec3_set(&m_world[ofs+0], -0.5f, -0.5f, z); // top left
    vec3_set(&m_world[ofs+1], 0.5f, -0.5f, z); // top right
    vec3_set(&m_world[ofs+2], 0.5f, 0.5f, z); // bottom right
    vec3_set(&m_world[ofs+3], -0.5f, 0.5f, z); // bottom left

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
        vec3_copy(&w, &m_world[i]);
        vec3_sub(&w, &w, &s_camera);

        z = 1.0f / w.z;
        s.x = X_CENTRE + (w.x * s_projXscale * z);
        s.y = Y_CENTRE + (w.y * s_projYscale * z);
        s.z = z;

        vec3_copy(&m_screen[i], &s);
    }
}

static void renderTunnel()
{
    int i;
    BYTE* buffer = video_getOffscreenBuffer();

    for (i = 1; i < s_numSegments; i++)
    {
        int j;
        int prevSegment = (i - 1) * 4;
        int currSegment = i * 4;
        BYTE prevColour = (BYTE)i; //(BYTE)((m_screen[prevSegment].z * 127.0f) + (((i >> 2) & 1) << 7));
        BYTE currColour = prevColour; //(BYTE)((m_screen[currSegment].z * 127.0f) + (((i >> 2) & 1) << 7));

        for (j = 0; j < 4; j++)
        {
            int ofs = (3 + j) & 3;
            Vector3 a, b, c;

            vec3_copy(&a, &m_screen[prevSegment + j]);
            vec3_copy(&b, &m_screen[prevSegment + ofs]);
            vec3_copy(&c, &m_screen[currSegment + j]);
            //if (a.z > 0 && b.z > 0 && c.z > 0)
                poly_draw(&a, prevColour, &b, prevColour, &c, currColour, buffer);

            vec3_copy(&a, &m_screen[currSegment + j]);
            vec3_copy(&b, &m_screen[prevSegment + ofs]);
            vec3_copy(&c, &m_screen[currSegment + ofs]);
            //if (a.z > 0 && b.z > 0 && c.z > 0)
                poly_draw(&a, currColour, &b, prevColour, &c, currColour, buffer);
        }
    }
}

static int init()
{
    float z = 1.2f;
    int i;
    for (i = 0; i < MAX_SEGMENTS; i++)
    {
        addSegment(z);
        z += 1.5f;
    }

    vec3_set(&s_camera, 0, 0, 0);

    return 1;
}

static void start()
{
    int i;
    for (i = 1; i <= 128; i++)
    {
        if ((i & 1) == 1)
        {
            video_setPal((BYTE)i, 63, 0, 0);
        }
        else
        {
            video_setPal((BYTE)i, 0, 32, 0);
        }
        // video_setPal((BYTE)i, (BYTE)(i >> 2), (BYTE)(i >> 2), (BYTE)(i >> 1));
        // video_setPal((BYTE)(i + 128), (BYTE)(i >> 1), (BYTE)(i >> 2), (BYTE)(i >> 8));
    }
}

static void update(float dt)
{
    s_projXscale = SCREEN_WIDTH / (2.0f * (float)tan(s_fovHori * 0.5f * PI / 180.0f));
    s_projYscale = s_projXscale * ASPECT;

    projectTunnel();
    renderTunnel();

    // {
    //     Vector3 a, b, c;
    //     vec3_set(&a, 211, 142.7f, 0);
    //     vec3_set(&b, 275, 3.7f, 0);
    //     vec3_set(&c, 211, 57, 0);
    //     poly_draw(&a, 127, &b, 127, &c, 127, video_getOffscreenBuffer());
    // }

    if (kb_keyDown(Key_Up))
    {
        s_camera.z += dt;
    }
    else if (kb_keyDown(Key_Down))
    {
        s_camera.z -= dt;
    }
}

static EffectDesc s_desc = { init, update, start, 0, 1 };

EffectDesc* polytun_getEffectDesc()
{
    return &s_desc;
}

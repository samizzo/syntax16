#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "plasma.h"
#include "video.h"
#include "image.h"

static BYTE* palette;
static int s_timer = 0;
static int px0, py0, px1, py1, px2, py2;
static BYTE* s_plasma0, *s_plasma1;

static int init()
{
    int i, j, ofs = 0;
    s_plasma0 = (BYTE*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2 * 2);
    s_plasma1 = (BYTE*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2 * 2);

    for (j = 0; j < SCREEN_HEIGHT*2; j++)
    {
        for (i = 0; i < SCREEN_WIDTH*2; i++)
        {
            // Borrowed (shamelessly stolen from) http://www.flipcode.com/archives/The_Art_of_Demomaking-Issue_04_Per_Pixel_Control.shtml
            s_plasma0[ofs] = (BYTE)( 64 + 63 * ( sin( (double)hypot( 200-j, 320-i )/16 ) ) );
            s_plasma1[ofs] = (BYTE)( 64 + 63 * sin( (float)i/(37+15*cos((float)j/74)) )* cos( (float)j/(31+11*sin((float)i/57))) );
            ofs++;
        }
    }

    return 1;
}

static void start()
{
    palette = image_loadPalette("data/plaspal.pal");
    video_setPalette(palette);

    // int i;
    // for (i = 0; i < 256; i++)
    // {
    //     video_setPal((BYTE)i, (unsigned char)(32 + 31 * cos( i * PI / 128 + (double)0/74 )),
    //                       (unsigned char)(32 + 31 * sin( i * PI / 128 + (double)0/63 )),
    //                     (unsigned char)(32 - 31 * cos( i * PI / 128 + (double)0/81 )) );
    // }
}

static void update(float dt)
{
    int x, y, ofs = 0;
    int src0, src1, src2;
    BYTE* buffer = video_getOffscreenBuffer();

    px0 = 160 + (int)(159.0f * cos( (double)s_timer/97 ));
    px1 = 160 + (int)(159.0f * sin( (double)-s_timer/114 ));
    px2 = 160 + (int)(159.0f * sin( (double)-s_timer/137 ));
    py0 = 100 + (int)(99.0f * sin( (double)s_timer/123 ));
    py1 = 100 + (int)(99.0f * cos( (double)-s_timer/75 ));
    py2 = 100 + (int)(99.0f * cos( (double)-s_timer/108 ));

    src0 = py0*640+px0;
    src1 = py1*640+px1;
    src2 = py2*640+px2;

    for (y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (x = 0; x < SCREEN_WIDTH; x++)
        {
            buffer[ofs] = s_plasma0[src0]+s_plasma1[src1]+s_plasma1[src2];
            ofs++; src0++; src1++; src2++;
        }
        src0 += SCREEN_WIDTH;
        src1 += SCREEN_WIDTH;
        src2 += SCREEN_WIDTH;
    }

    //video_drawPalette();

    //s_timer += dt * 20.0f;
    s_timer += 2;;
}

static EffectDesc s_desc = { init, update, start, 0, 1 };

EffectDesc* plasma_getEffectDesc()
{
    return &s_desc;
}

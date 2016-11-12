#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "funkyln.h"
#include "video.h"

static int s_timer = 0;
static BYTE* s_arctan, *s_sqrt;

static int init()
{
    int i, j, ofs = 0;
    s_arctan = (BYTE*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2 * 2);
    s_sqrt = (BYTE*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2 * 2);

    for (j = 0; j < SCREEN_HEIGHT*2; j++)
    {
        for (i = 0; i < SCREEN_WIDTH*2; i++)
        {
            int y = j-200, x = i-320;
            s_arctan[ofs] = (unsigned char)(atan2((float)y, (float)x) * 256.f / PI);
            s_sqrt[ofs] = (unsigned char)sqrt((float)x*x + y*y);
            ofs++;
        }
    }

    return 1;
}

static void start()
{
    #define setPal(i, r, g, b) pal[(i)*3] = (r)>>2; pal[((i)*3)+1] = (g)>>2; pal[((i)*3)+2] = (b)>>2

    int i;
    BYTE pal[256*3];

    for (i = 0; i < 32; i++)
    {
        // r=0, g=159, b=255 to r=3, g=189, b=40
        setPal(i, (0+(3*i)/32), ((159+(189-159)*i/32)), (255+(40-255)*i/32));

        // r=3, g=189, b=40 to r=227, g=245, b=3
        setPal(32+i, ((3+(227-3)*i/32)), ((189+(245-189)*i/32)), (40+(3-40)*i/32));

        // r=227, g=245, b=3 to r=251, g=173, b=102
        setPal(64+i, ((227+(251-227)*i/32)), ((245+(173-245)*i/32)), (3+(102-3)*i/32));

        // r=251, g=173, b=102 to r=232, g=0, b=40
        setPal(96+i, ((251+(232-251)*i/32)), ((173+(0-173)*i/32)), (102+(40-102)*i/32));

        // r=232, g=0, b=40 to r=206, g=22, b=233
        setPal(128+i, ((232+(206-232)*i/32)), ((22*i/32)), (40+(233-40)*i/32));

        // r=206, g=22, b=233 to r=133, g=15, b=240
        setPal(160+i, ((206+(133-206)*i/32)), ((22+(15-22)*i/32)), (233+(240-233)*i/32));

        // r=133, g=15, b=240 to r=50, g=120, b=205
        setPal(192+i, ((133+(50-133)*i/32)), ((15+(120-15)*i/32)), (240+(205-240)*i/32));

        // r=60, g=120, b=205 to r=0, g=159, b=255
        setPal(224+i, ((60+(0-60)*i/32)), ((120+(159-120)*i/32)), (205+(255-205)*i/32));
    }

    video_setPalette(pal);
}

static void update(float dt)
{
    int i, j;
    BYTE* buffer = video_getOffscreenBuffer();
    BYTE* table = s_arctan;
    int d = 0;
    int src0, src1, src2;
    int px0, py0, px1, py1, px2, py2;

    px0 = 160 + (int)(150.0f * cos( (double)s_timer/97 ));
    py0 = 100 + (int)(99.0f * sin( (double)s_timer/123 ));

    px1 = 160 + (int)(139.0f * sin( (double)-s_timer/114 ));
    py1 = 100 + (int)(59.0f * cos( (double)-s_timer/75 ));

    px2 = 160 + (int)(109.0f * sin( (double)-s_timer/137 ));
    py2 = 100 + (int)(29.0f * cos( (double)-s_timer/108 ));

    src0 = px0 + (py0*640);
    src1 = px1 + (py1*640);
    src2 = px2 + (py2*640);

    for (j = 0; j < 200; j++)
    {
        for (i = 0; i < 320; i++)
        {
            int aa;
            aa = table[src0] + table[src1] + table[src2];
            src0++;
            src1++;
            src2++;
            buffer[d++] = (BYTE)aa;
        }

        src0 += SCREEN_WIDTH;
        src1 += SCREEN_WIDTH;
        src2 += SCREEN_WIDTH;
    }

    //video_drawPalette();

    //s_timer += dt * 20.0f;
    s_timer += 4;
}

static EffectDesc s_desc = { init, update, start, 0, 1 };

EffectDesc* funkyln_getEffectDesc()
{
    return &s_desc;
}

#include "video.h"
#include <stdlib.h>

static BYTE* s_buffer;

void video_init()
{
    __asm
    {
        mov ax, 13h
        int 10h
    }

    s_buffer = (BYTE*)malloc(SCREEN_WIDTH*SCREEN_HEIGHT);
}

void video_deinit()
{
    free(s_buffer);
    s_buffer = 0;

    __asm
    {
        mov ax, 03h
        int 10h
    }
}

void video_waitForRetrace()
{
    __asm
    {
         mov dx, 3dah
        l1:
         in al, dx
         and al, 08h
         jz l1
        l2:
         in al, dx
         and al, 08h
         jnz l2
    }
}

BYTE* video_getOffscreenBuffer()
{
    return s_buffer;
}

void video_setPal(BYTE col, BYTE r, BYTE g, BYTE b)
{
    __asm
    {
        mov dx, 0x3c8
        mov al, col
        out dx, al
        inc dx
        mov al, r
        out dx, al
        mov al, g
        out dx, al
        mov al, b
        out dx, al

    }
}

void video_setPalette(BYTE* palette)
{
    __asm
    {
        mov dx, 0x3c8
        mov al, 0
        out dx, al
        inc dx

        mov cx, 256*3
        mov esi, palette

    looper:
        mov al, byte ptr [esi]
        out dx, al
        inc esi
        dec cx
        jnz looper
    }
}

void video_drawPalette()
{
    int x, y;
    BYTE* buffer = video_getOffscreenBuffer();
    for (x = 0; x < 256; x++)
    {
        for (y = 0; y < 10; y++)
        {
            buffer[x + (y * SCREEN_WIDTH)] = (BYTE)x;
        }
    }
}

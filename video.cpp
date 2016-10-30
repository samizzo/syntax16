#include "video.h"
#include <stdlib.h>

static BYTE* s_buffer;

namespace video
{

void init()
{
    __asm
    {
        mov ax, 13h
        int 10h
    }

    s_buffer = (BYTE*)malloc(320*200);
}

void deinit()
{
    free(s_buffer);
    s_buffer = 0;

    __asm
    {
        mov ax, 03h
        int 10h
    }
}

void waitForRetrace()
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

void flip(void *src, void *dest, DWORD size)
{
    __asm
    {
        mov esi, src
        mov edi, dest
        mov ecx, size
        rep movsd
    }
}

void clear(void *dest, DWORD col, DWORD size)
{
    __asm
    {
        mov edi, dest
        mov eax, col
        mov ecx, size
        rep stosd
    }
}

BYTE* getOffscreenBuffer()
{
    return s_buffer;
}

void setPal(BYTE col, BYTE r, BYTE g, BYTE b)
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

void setPal(BYTE* palette)
{
    for (int i = 0; i < 256; i++, palette += 3)
        setPal((BYTE)i, palette[0], palette[1], palette[2]);
}

}

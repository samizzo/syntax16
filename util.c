#include "util.h"

DWORD g_seed;

void util_blit(void *src, void *dest, DWORD size)
{
    __asm
    {
        mov esi, src
        mov edi, dest
        mov ecx, size
        rep movsd
    }
}

void util_clear(void *dest, DWORD col, DWORD size)
{
    __asm
    {
        mov edi, dest
        mov eax, col
        mov ecx, size
        rep stosd
    }
}

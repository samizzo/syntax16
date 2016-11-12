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

// void timer_init()
// {
//     DWORD foo = 2;
//     __asm
//     {
//         fild    dword ptr [foo]
//         mov     ecx,cs:[046Ch]
//         db      0Fh, 31h
//         push    edx
//         push    eax
//         fild    qword ptr [esp]
//         fchs
//         add     esp,8
//         add     ecx,18*2
// @@wait2:
//         mov     ebx,cs:[046Ch]
//         cmp     ebx,ecx
//         jb      @@wait2
//         db      0Fh, 31h
//         push    edx
//         push    eax
//         fild    qword ptr [esp]
//         add     esp,8
//         faddp   st(1), st
//         fdivrp  st(1)
//         fstp    qword ptr [_TIME_CONSTANT]
//     }
// }

// long double timer_get_time()
// {
// }

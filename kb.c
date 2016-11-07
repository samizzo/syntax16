#include "kb.h"

char kb_keyDown(enum Key key)
{
    BYTE keyPress = 0;

    __asm
    {
        in al, 60h
        mov keyPress, al
    }

    return keyPress == (BYTE)key;
}

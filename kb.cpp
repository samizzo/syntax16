#include "kb.h"

namespace kb
{

bool keyDown(Key key)
{
    BYTE keyPress;

    __asm
    {
        in al, 60h
        mov keyPress, al
    }

    return keyPress == (BYTE)key;
}

}
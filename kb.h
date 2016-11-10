#ifndef _kb_h
#define _kb_h

#include "util.h"

enum Key
{
    Key_Escape = 1,
    Key_Up = 0x48,
    Key_Down = 0x50,
    Key_Left = 0x4b,
    Key_Right = 0x4d
};

char kb_keyDown(enum Key key);

#endif

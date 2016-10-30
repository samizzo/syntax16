#ifndef _kb_h
#define _kb_h

#include "types.h"

namespace kb
{
    enum Key
    {
        Escape = 1,
        Up = 0x48,
        Down = 0x50,
        Left = 0x4b,
        Right = 0x4d
    };

	bool keyDown(Key key);
}

#endif

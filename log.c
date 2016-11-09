#include "log.h"
#include <stdio.h>
#include <stdarg.h>

#if defined(DEBUG)
void log_debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
#endif

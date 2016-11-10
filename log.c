#include "log.h"
#include <stdio.h>
#include <stdarg.h>

#if defined(DEBUG)
static FILE* s_fp = 0;

void log_init(const char* file)
{
    s_fp = fopen(file, "w");
}

void log_shutdown()
{
    fclose(s_fp);
}

void log_debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (s_fp)
    {
        vfprintf(s_fp, format, args);
        //fflush(s_fp);
    }
    else
    {
        vprintf(format, args);
    }
    va_end(args);
}
#endif

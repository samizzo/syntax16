#ifndef _log_h
#define _log_h

#if defined(DEBUG)
void log_debug(const char* format, ...);
#else
#define log_debug(...) do { } while (0)
#endif

#endif

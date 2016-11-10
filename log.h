#ifndef _log_h
#define _log_h

#if defined(DEBUG)
void log_init(const char* file);
void log_shutdown();
void log_debug(const char* format, ...);
#else
#define log_debug(...) do { } while (0)
#define log_init(file) do { } while (0)
#define log_shutdown()
#endif

#endif

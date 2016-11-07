#ifndef _video_h
#define _video_h

#include "types.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void video_init();
void video_deinit();
void video_waitForRetrace();
void video_flip(void* src, void* dst, DWORD size);
void video_clear(void *dest, DWORD col, DWORD size);

BYTE* video_getOffscreenBuffer();

// Set an individual palette entry.
void video_setPal(BYTE col, BYTE r, BYTE g, BYTE b);

#endif

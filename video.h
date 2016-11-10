#ifndef _video_h
#define _video_h

#include "util.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void video_init();
void video_deinit();
void video_waitForRetrace();

// Copy src to dst. Size is in dwords.
void video_flip(void* src, void* dst, DWORD size);

// Clear dest to col (8 pixels at once). Size is in dwords.
void video_clear(void *dest, DWORD col, DWORD size);

BYTE* video_getOffscreenBuffer();

void video_setPal(BYTE col, BYTE r, BYTE g, BYTE b);
void video_setPalette(BYTE* palette);
void video_drawPalette();

#endif

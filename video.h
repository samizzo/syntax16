#ifndef _video_h
#define _video_h

#include "util.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void video_init();
void video_deinit();
void video_waitForRetrace();

BYTE* video_getOffscreenBuffer();

void video_setPal(BYTE col, BYTE r, BYTE g, BYTE b);
void video_setPalette(BYTE* palette);
void video_drawPalette();

#endif

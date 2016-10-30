#ifndef _video_h
#define _video_h

#include "types.h"

namespace video
{
	void init();
	void deinit();
	void waitForRetrace();
	void flip(void* src, void* dst, DWORD size);
	void clear(void *dest, DWORD col, DWORD size);

	BYTE* getOffscreenBuffer();

	// Set an individual palette entry.
	void setPal(BYTE col, BYTE r, BYTE g, BYTE b);

	// Set an entire palette at once. 'palette' is an array of 256 RGB byte values.
	void setPal(BYTE* palette);
}

#endif

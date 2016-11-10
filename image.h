#ifndef _image_h
#define _image_h

#include "util.h"

typedef struct Image_tag
{
    BYTE* pixels;
    BYTE* palette;
    DWORD width;
    DWORD height;
} Image;

Image* image_loadFromTGA(const char* path);
Image* image_create(int width, int height);
void image_destroy(Image* image);

BYTE* image_loadPalette(const char* path);
void image_destroyPalette(BYTE* palette);

// Remap the given image's palette so the colours are linear.
void image_remapPaletteLinear(Image* image);

#endif

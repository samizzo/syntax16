#ifndef _image_h
#define _image_h

#include "types.h"

typedef struct Image_tag
{
    BYTE* pixels;
    BYTE* palette;
    DWORD width;
    DWORD height;
} Image;

Image* image_loadFromTGA(const char* path);
void image_destroy(Image* image);

#endif

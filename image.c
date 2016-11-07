#include "image.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Header_tag
{
    BYTE idLength;
    BYTE colourMapType;
    BYTE imageType;

    // Colour map spec
    WORD colourMapFirstEntryIndex;
    WORD colourMapLength;
    BYTE colourMapBitsPerPixel;

    // Image spec
    WORD xorigin;
    WORD yorigin;
    WORD width;
    WORD height;
    BYTE bitsPerPixel;
    BYTE imageDesc;
} Header;

static Header header;

Image* image_loadFromTGA(const char* path)
{
    Image* image = 0;
    FILE* fp = fopen(path, "rb");
    if (fp == 0)
    {
        log_debug("Couldn't load file %s\n", path);
        return 0;
    }

    fread(&header, sizeof(header), 1, fp);

    printf("colourMapType=%i\n", header.colourMapType);
    printf("imageType=%i\n", header.imageType);
    printf("colourMapLength=%i\n", header.colourMapLength);
    printf("colourMapBitsPerPixel=%i\n", header.colourMapBitsPerPixel);

    // We only support colour mapped images.
    if (header.colourMapType != 1)
        return 0;

    // We only support uncompressed colour mapped images.
    if (header.imageType != 1)
        return 0;

    image = (Image*)malloc(sizeof(Image));
    return image;
}

void image_destroy(Image* image)
{
    free(image->pixels);
    free(image->palette);
    free(image);
}

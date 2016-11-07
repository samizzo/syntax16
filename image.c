#include "image.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
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
#pragma pack(pop)

static Header header;

Image* image_loadFromTGA(const char* path)
{
    Image* image = 0;
    int i;
    FILE* fp = fopen(path, "rb");
    if (fp == 0)
    {
        log_debug("Couldn't load file %s\n", path);
        return 0;
    }

    fread(&header, sizeof(header), 1, fp);

    // log_debug("colourMapType=%i\n", header.colourMapType);
    // log_debug("imageType=%i\n", header.imageType);
    // log_debug("colourMapLength=%i\n", header.colourMapLength);
    // log_debug("colourMapBitsPerPixel=%i\n", header.colourMapBitsPerPixel);

    // We only support colour mapped images.
    if (header.colourMapType != 1)
        return 0;

    // We only support uncompressed colour mapped images.
    if (header.imageType != 1)
        return 0;

    // We only support 24-bit colour mapped images.
    if (header.colourMapBitsPerPixel != 24)
        return 0;

    // Colour map must always be 256 entries in size.
    if (header.colourMapLength != 256)
        return 0;

    log_debug("Image is %ix%i\n", header.width, header.height);

    image = (Image*)malloc(sizeof(Image));
    image->width = header.width;
    image->height = header.height;
    image->pixels = (BYTE*)malloc(image->width * image->height);

    // Skip image id
    fseek(fp, header.idLength, SEEK_CUR);

    // Read the palette
    image->palette = (BYTE*)malloc(256 * 3);

    {
        BYTE* pal = image->palette;
        for (i = 0; i < header.colourMapLength; i++, pal += 3)
        {
            fread(pal, 3, 1, fp);

            // Convert to 6 bit
            pal[0] = pal[0] >> 2;
            pal[1] = pal[1] >> 2;
            pal[2] = pal[2] >> 2;
        }
    }

    // Read the image data
    fread(image->pixels, image->width * image->height, 1, fp);

    return image;
}

void image_destroy(Image* image)
{
    free(image->pixels);
    free(image->palette);
    free(image);
}

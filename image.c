#include "image.h"
#include "vector3.h"
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

    // Colour map must be max 256 entries in size.
    if (header.colourMapLength > 256)
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
    memset(image->palette, 0, 256 * 3);

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

void image_remapPaletteLinear(Image* image)
{
    BYTE indices[256];
    int i, j;

    for (i = 0; i < 256; i++)
        indices[i] = i;

    for (j = 0; j < 256 - 1; j++)
    {
        int min = j;
        for (i = j + 1; i < 256; i++)
        {
            Vector3 a, b;
            float alen, blen;

            a.x = image->palette[i*3];
            a.y = image->palette[(i*3)+1];
            a.z = image->palette[(i*3)+2];
            b.x = image->palette[min*3];
            b.y = image->palette[(min*3)+1];
            b.z = image->palette[(min*3)+2];

            alen = vec3_len(a);
            blen = vec3_len(b);
            if (alen < blen)
                min = i;
        }

        if (min != j)
        {
            BYTE s;
            BYTE c[3];
            memcpy(c, &image->palette[j*3], 3);
            memcpy(&image->palette[j*3], &image->palette[min*3], 3);
            memcpy(&image->palette[min*3], c, 3);

            s = indices[j];
            indices[j] = indices[min];
            indices[min] = s;
        }
    }

    for (i = 0; i < image->width * image->height; i++)
    {
        BYTE c = image->pixels[i];
        for (j = 0; j < 256; j++)
        {
            if (indices[j] == c)
                break;
        }
        image->pixels[i] = j;
    }
}

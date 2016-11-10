#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long int DWORD;

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
static FILE* fp = 0;
static BYTE* palette = 0;
static int i;

void bye(const char* msg)
{
    if (msg)
        printf(msg);
    if (palette)
        free(palette);
    if (fp)
        fclose(fp);
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("usage: pt <filename.tga> <output.pal>\n");
        return 0;
    }

    fp = fopen(argv[1], "rb");
    if (fp == 0)
    {
        printf("couldn't open %s\n", argv[1]);
        return 0;
    }

    fread(&header, sizeof(header), 1, fp);

    if (header.colourMapType != 0)
    {
        bye("file has colour map\n");
    }

    // We only support uncompressed truecolour images.
    if (header.imageType != 2)
    {
        bye("unsupported file type\n");
    }

    if (header.colourMapLength != 0)
    {
        bye("file has colour map\n");
    }

    if (header.bitsPerPixel != 24)
    {
        bye("unsupported pixel depth\n");
    }

    if (header.width != 256)
    {
        bye("image is not 256 pixels wide\n");
    }

    printf("Image is %ix%i\n", header.width, header.height);

    // Skip image id
    fseek(fp, header.idLength, SEEK_CUR);

    palette = (BYTE*)malloc(256*3);

    printf("Reading palette from first row\n");

    for (i = 0; i < 256; i++)
    {
        //fread(image->pixels, image->width * image->height, 1, fp);
        BYTE r, g, b;
        fread(&b, 1, 1, fp);
        fread(&g, 1, 1, fp);
        fread(&r, 1, 1, fp);

        palette[i*3] = r >> 2;
        palette[(i*3)+1] = g >> 2;
        palette[(i*3)+2] = b >> 2;
    }

    printf("Writing palette\n");
    fclose(fp);
    fp = fopen(argv[2], "wb");
    fwrite(palette, 256*3, 1, fp);
    fclose(fp);
    fp = 0;

    bye("Done");
    return 0;
}

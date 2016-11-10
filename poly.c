#include "poly.h"
#include "video.h"

#define swap(x, y) x = x ^ y; y = y ^ x; x = x ^ y

void hline(int x1, int col1, int x2, int col2, int y, BYTE* buffer)
{
    int i;
    float cgrad, cer;

    if (x2 < x1)
    {
        swap(x2, x1);
        swap(col2, col1);
    }

    if (x2 < 0)
        return;

    if (x1 >= SCREEN_WIDTH)
        return;

    if ((x2 - x1) == 0)
    {
        buffer[x1 + (y*SCREEN_WIDTH)] = col1;
        return;
    }

    cgrad = (col2 -col1) / (float)(x2 - x1);
    cer = col1;
    if (x1 < 0)
    {
        cer = cer + (-x1 * cgrad);
        x1 = 0;
    }

    buffer += x1 + (y * SCREEN_WIDTH);
    if (x2 >= SCREEN_WIDTH)
        x2 = SCREEN_WIDTH - 1;;

    for (i = x1; i < x2; i++)
    {
        *buffer = (BYTE)cer;
        cer += cgrad;
        buffer++;
    }

    // asm
    //     mov di, d
    //     mov cx, word ptr x2
    //     sub cx, word ptr x1

    //     mov ax, virscr
    //     mov es, ax
    //     mov ax, cer
    //     mov bx, cgrad

    //     @drawloop:
    //     mov es:[di], ah
    //     add ax, bx
    //     inc di
    //     dec cx
    //     jns @drawloop
    // end;
}

void poly_draw(Vector3* p1, BYTE c1, Vector3* p2, BYTE c2, Vector3* p3, BYTE c3, BYTE* buffer)
{
    float cgrad1,
    cgrad2,
    cgrad3,
    cer1,
    cer2,
    xgrad1,
    xgrad2,
    xgrad3,
    xer1, xer2;
    int temp1, temp2, temp3;
    int i, adder;
    int x1, y1, x2, y2, x3, y3;

    x1 = (int)p1->x;
    y1 = (int)p1->y;
    x2 = (int)p2->x;
    y2 = (int)p2->y;
    x3 = (int)p3->x;
    y3 = (int)p3->y;

    // buffer[x1 + (y1*320)] = c1;
    // buffer[x2 + (y2*320)] = c2;
    // buffer[x3 + (y3*320)] = c3;

    if (y2 > y3)
    {
        swap(y2, y3);
        swap(x2, x3);
        swap(c2, c3);
    }

    if (y1 > y2)
    {
        swap(y2, y1);
        swap(x2, x1);
        swap(c2, c1);
    }

    if (y2 > y3)
    {
        swap(y2, y3);
        swap(x2, x3);
        swap(c2, c3);
    }

    // is entire polygon out of screen?
    if (y3 < 0)
        return;

    // is entire polygon out of screen?
    if (y1 >= SCREEN_HEIGHT)
        return;

    temp2 = y3 - y1;

    // is polygon empty area?
    if (temp2 == 0)
        return;

    temp1 = y2 - y1;
    temp3 = y3 - y2;

    xgrad2 = (x3 - x1) / (float)temp2;
    cgrad2 = (c3 - c1) / (float)temp2;

    // if top is horizontal
    if (temp1 == 0)
    {
        xgrad1 = (x3 - x2) / (float)temp3;
        cgrad1 = (c3 - c2) / (float)temp3;

        xer2 = x1;
        xer1 = x2;
        cer2 = c1;
        cer1 = c2;

        adder = 0;
    }
    else
    {
        // if top isn't horizontal
        adder = 0; // 1?
        xgrad1 = (x2 - x1) / (float)temp1;
        cgrad1 = (c2 - c1) / (float)temp1;

        cer1 = c1;
        cer2 = cer1;

        xer1 = x1;
        xer2 = xer1;
    }

    // if not horizontal on the bottom. otherwise we'll abort the last loop
    if (temp3 != 0)
    {
        xgrad3 = (x3 - x2) / (float)temp3;
        cgrad3 = (c3 - c2) / (float)temp3;
    }

    if (y2 < 0)
    {
        // this means y1 < 0

        xer1 = x2; // to eliminate fractional errors (??)
        cer1 = c2;

        xer1 = xer1 - (y2 * xgrad3);
        cer1 = cer1 - (y2 * cgrad3);

        xer2 = xer2 - (y1 * xgrad2);
        cer2 = cer2 - (y1 * cgrad2);

        xgrad1 = xgrad3;
        cgrad1 = cgrad3;

        adder = 0;
        y2 = 0;
    }
    else
    {
        // y2 isn't < 0 but y1 may be
        if (y1 < 0)
        {
            // step along from y1 to 0
            xer1 = xer1 - (y1 * xgrad1);
            xer2 = xer2 - (y1 * xgrad2);
            cer1 = cer1 - (y1 * cgrad1);
            cer2 = cer2 - (y1 * cgrad2);
            y1 = 0;
        }

        if (y2 >= SCREEN_HEIGHT)
            y2 = SCREEN_HEIGHT - 1;;

        // if not horizontal on top, do first fill
        if (temp1 != 0)
        {
            for (i = y1; i < y2; i++)
            {
                hline((int)xer1, (int)cer1, (int)xer2, (int)cer2, i, buffer);

                xer1 += xgrad1;
                xer2 += xgrad2;

                cer1 += cgrad1;
                cer2 += cgrad2;
            }

            xgrad1 = xgrad3;
            cgrad1 = cgrad3;
            //xer1 = x2 + xgrad1; // to eliminate fractional errors
            //cer1 = c2 + cgrad1;
        }
    }

    if (y3 >= SCREEN_HEIGHT)
        y3 = SCREEN_HEIGHT - 1;

    // if not horizontal on bottom, fill it
    if (temp3 != 0)
    {
        for (i = y2+adder; i < y3; i++)
        {
            hline((int)xer1, (int)cer1, (int)xer2, (int)cer2, i, buffer);

            xer1 += xgrad1;
            xer2 += xgrad2;

            cer1 += cgrad1;
            cer2 += cgrad2;
        }
    }
}

#include "thing.h"
#include "info.h"
#include "drawer.h"
#include <cmath>

using namespace std;

void Drawer::drawPixel(int ix, int iy)
{
    long int location = (ix+Info::vinfo.xoffset) * Info::bytePerX+ (iy+Info::vinfo.yoffset)*Info::bytePerY;
    if (Info::vinfo.bits_per_pixel == 32) {
        *(Info::fbp + location) = 255;        // Some blue
        *(Info::fbp + location + 1) = 255 ;     // A little green
        *(Info::fbp + location + 2) = 255 ;    // A lot of red
        *(Info::fbp + location + 3) = 0;      // No transparency
    } else  { //assume 16bpp
        int c = 10;
        int g = (((21)-100)/6 )%255;     // A little green
        int r = (31-((132)-100)/16) %255;    // A lot of red
        unsigned short int t = r<<11 | g << 5 | c;
        *((unsigned short int*)(Info::fbp + location)) = t;
    }
}

void Drawer::drawLine(int x0, int x1, int y0, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x, y;
    int err, e2;

    int sx = x0 < x1 ? 1:-1;
    int sy = y0 < y1 ? 1:-1;
    err = dx - dy; x = x0; y = y0;
    for (;;) {
        drawPixel(x, y);
        if (x == x1 && y == y1) break;
        e2 = err << 1;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }

        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void Drawer::drawBox(Box *b)
{
    int ix;             /* Sumbu X */
    int iy;             /* Sumbu Y */
    long int location;  /* Point's current location */

    for (ix = b->offX; ix < b->offX + b->x; ix++) {
        for (iy = b->offY; iy < b->offY + b->y; iy++) {
            location = (ix+Info::vinfo.xoffset) * Info::bytePerX + (iy+Info::vinfo.yoffset)*Info::bytePerY;
            if (Info::vinfo.bits_per_pixel == 32) {
                *(Info::fbp + location) = (b->rgb)[2]; /* Blue */
                *(Info::fbp + location + 1) = (b->rgb)[1]; ;     /* Green */
                *(Info::fbp + location + 2) = (b->rgb)[0]; ;    /* Red */
                *(Info::fbp + location + 3) = 0;      /* Alpha */
            } else  { //assume 16bpp
                int c = 10;
                int g = (((b->x)-100)/6 )%255;     // A little green
                int r = (31-((b->y)-100)/16) %255;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | c;
                *((unsigned short int*)(Info::fbp + location)) = t;
            }
        }
    }
}

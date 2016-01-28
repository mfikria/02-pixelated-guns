#ifndef _DRAWER_
#define _DRAWER_

#include "thing.h"

class Drawer {
    public:
        static void drawPixel(int ix, int iy);
        static void drawCircle(int xm, int ym, int r);
        static void drawLine(int x0, int x1, int y0, int y1);
        static void drawBox(Box *box);
        static void clearScreen();
};

#endif



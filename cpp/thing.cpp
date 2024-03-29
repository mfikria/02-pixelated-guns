#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cmath>
#include "thing.h"
#include "drawer.h"
#include "info.h"

using namespace std;

Thing::Thing()
{
    this->offX = 0;
    this->offY = 0;
    this->x = 0;
    this->y = 0;
    this->nBox = 0;

    this->minX = Info::xres - 10;
    this->minY = Info::yres - 10;
    this->maxX = 0;
    this->maxY = 0;

}

/* METHODS */
void Thing::draw()
{
    for (int i = 0; i < nBox; i++) {
        Drawer::drawBox(boxes + i);
    }
}

void Thing::moveX(int dx, int speed)
{
    int bx = Info::bytePerX;
    int by = Info::bytePerY;
    char *fbp = Info::fbp;
    int move = 1;

    for (int k = 0; k < abs(dx); k++) {
        long int initial_location = offX*bx + offY*by;
        if (dx > 0) {
            int temp[x*4];
            for (int i = 0; i < y; i++) {
                memcpy(temp, fbp + initial_location + (i * by), x*bx);
                memcpy(fbp + initial_location + (i * by) + move*bx, temp, x*bx);
                memset(fbp + initial_location + (i * by), 0, bx);
            }
            offX++;
            usleep(speed);
        } else {
            for (int i = 0; i < y; i++) {
                memcpy(fbp + initial_location + (i * by) - move*bx, fbp + initial_location + (i * by), 4);
                memset(fbp + initial_location + (i * by) + x*bx, 0, bx);
            }
            offX--;
            usleep(speed);
        }

    }
}


void Thing::moveY(int dy, int speed)
{
    /* Lokasi awal / offset awal */
    int bx = Info::bytePerX;
    int by = Info::bytePerY;
    char *fbp = Info::fbp;
    int move = 1;
    /* finished_location : lokasi setelah iterasi ini berakhir */
    long int initial_location, finished_location;
    for (int i = 0; i < abs(dy); i++)
    {
        initial_location = offX*bx + offY*by;
        if (dy > 0) {
            finished_location = initial_location + (move * by);
            for (int i = y - 1; i >= 0; i--) {
                memcpy(fbp + finished_location + (i * by), fbp + initial_location + (i * by), x*bx);
            }
            memset(fbp + initial_location, 0, bx*x);
            offY++;
            usleep(speed);
        } else if (dy < 0){
            finished_location = initial_location - (move *by);
            for (int i = 0; i < y; i++) {
                memcpy(fbp + finished_location + (i * by), fbp + initial_location + (i * by), x*bx);
            }
            memset(fbp + initial_location + (y * by), 0, bx*x);
            offY--;
            usleep(speed);
        }
    }
}


/* SETTERS */
void Thing::addBox(int offX, int offY, int x, int y, int rgb[3])
{
    Box newBox = {offX, offY, x, y, {rgb[0], rgb[1], rgb[2]}};  /* Create new box */
    boxes[nBox] = newBox;                                       /* Append created one to boxes */
    nBox++; /* increment nBox */
}

void Thing::addBox(Box *box)
{
    boxes[nBox] = *box;

    /* Update Thing's Attributes */
    if (box->offX + box->x > maxX)
        maxX = box->offX + box->x;
    if (box->offX  < minX)
        minX = box->offX ;

    if (box->offY + box->y > maxY)
        maxY = box->offY + box->y;
    if (box->offY < minY)
        minY = box->offY;

    offX = minX;
    offY = minY;
    cout<<"Max X : "<<maxX<<", MinX : "<<minX<<endl;
    x = maxX - minX;
    y = maxY - minY;

    nBox++;
}

void Thing::move(int x0, int y0, int x1, int y1, int speed)
{
    /* Initial jarak dari x0 terhadap offset */
    int dxOff = offX - x0;

    /* Initial jarak dari y0 terhadap offset */
    int dyOff = offY - y0;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x, y;
    int err, e2;

    int sx = x0 < x1 ? 1:-1;
    int sy = y0 < y1 ? 1:-1;
    err = dx - dy; x = x0; y = y0;

    int counter = 0;

    int rgb[] = {0, 0, 0};

    for (;; counter++) {
        Drawer::drawPixel(offX - dxOff,offY - dyOff , rgb);
        moveX(x - abs(offX - dxOff), speed);
        moveY(y - abs(offY - dyOff), speed);
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


/* Return pointer to i-th element in boxes */
Box *Thing::getEl(int i)
{
    return (boxes + i);
}

int Thing::getOffX()
{
    return offX;
}

int Thing::getOffY()
{
    return offY;
}

int Thing::getX()
{
    return x;
}

int Thing::getY()
{
    return y;
}

#include <iostream>
#include <cstring>
#include "defs.h"

using namespace std;

Thing::Thing(int offX, int offY, int x, int y)
{
    this->offX = offX;
    this->offY = offY;
    this->x = x;
    this->y = y;
    this->nBox = 0;
}

/* METHODS */
void draw(int red=255, int green=255, int blue=255);
void moveX(int dx);
void moveY(int dy);



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
    nBox++;
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

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>

#include "info.h"


using namespace std;

Info::Info()
{}

int Info::fbfd;
long int Info::screensize;
int Info::xres;
int Info::yres;
int Info::bytePerX;
int Info::bytePerY;
struct fb_var_screeninfo Info::vinfo ;
struct fb_fix_screeninfo Info::finfo ;
char *Info::fbp;



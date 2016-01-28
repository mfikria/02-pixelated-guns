#include "info.h"
#include "thing.h"
#include "drawer.h"
#include <stdio.h>
#include <iostream>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cmath>
#include <cstdlib>

using namespace std;

/* Initialize screen information */
void initInfo();

int main()
{
    initInfo();

    Box b = {200, 200, 200, 300, {255, 255, 255}};
    // Drawer::drawBox(&b);

    Thing t;
    t.addBox(&b);

    t.draw();
    cout<<"x : "<<t.getX()<<endl;
    t.moveX(-100);

    // Unmap the device from memory
    munmap(Info::fbp, Info::screensize);
    close(Info::fbfd);

    return 0;
}


void initInfo()
{
    int xres;            /* Resolusi horizontal layar */
    int yres;            /* Resolusi vertical layar */
    int bytePerX;        /* Jumlah byte untuk menggeser 1 pixel horizontal */
    int bytePerY;        /* Jumlah byte untuk menggerer 1 pixel vertical */

    long int screensize;

    struct fb_var_screeninfo vinfo;         /* Var screen info */
    struct fb_fix_screeninfo finfo;         /* Fixed screen info */
    int fbfd;                           /* File descriptor */
    char *fbp; /* Open the frame buffer file for reading and writing */
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    cout<<"The framebuffer device was opened successfully"<<endl;


    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    /*****************************************************************/
    xres = vinfo.xres;
    yres = vinfo.yres;
    bytePerX = vinfo.bits_per_pixel / 8;
    bytePerY = finfo.line_length;
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /******************************************************************/

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((long int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }

    printf("The framebuffer device was mapped to memory successfully.\n");
    Info info;


    Info::screensize = screensize;
    Info::xres = xres;
    Info::yres = yres;
    Info::bytePerX = bytePerX;
    Info::bytePerY = bytePerY;
    Info::vinfo = vinfo;
    Info::finfo = finfo;
    Info::fbfd = fbfd;
    Info::fbp = fbp;
}

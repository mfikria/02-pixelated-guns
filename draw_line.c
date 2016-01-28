#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>

#define MAX_BOX 4096

/* Global Variable and Macros */
struct fb_var_screeninfo vinfo;         /* Var screen info */
struct fb_fix_screeninfo finfo;         /* Fixed screen info */
int fbfd = 0;                           /* File descriptor */
char *fbp = 0;                          /* pointer to first pixel address in fbfd*/

typedef struct box{
    int offsetx;        /* Offset x */
    int offsety;        /* Offset y */
    int x;              /* Width */
    int y;              /* Height */
} box;

typedef struct letter{
    box boxes[MAX_BOX];        /* Array of box */
    int count;                 /* Current count of boxes */
} letter;

void drawPixel(int ix, int iy) {
    long int location = (ix+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (iy+vinfo.yoffset)*finfo.line_length;
    if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = 100;        // Some blue
        *(fbp + location + 1) = 100 ;     // A little green
        *(fbp + location + 2) = 100 ;    // A lot of red
        *(fbp + location + 3) = 0;      // No transparency
    } else  { //assume 16bpp
        int c = 10;
        int g = (((21)-100)/6 )%255;     // A little green
        int r = (31-((132)-100)/16) %255;    // A lot of red
        unsigned short int t = r<<11 | g << 5 | c;
        *((unsigned short int*)(fbp + location)) = t;
    }
}

void drawLine(int x0, int x1, int y0, int y1) {
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

int main(int argc, char const *argv[])
{
    long int screensize = 0;

    // Open the frame buffer file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

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

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((long int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

	int x0, x1, y0, y1;
	scanf("%d %d %d %d",&x0, &x1, &y0, &y1);

	system("clear");
    drawLine(x0, x1, y0, y1);

    box line = {vinfo.xoffset, y0, abs(vinfo.xoffset-vinfo.xres), abs(y1-y0+1)};
    move_box(&line, NULL, 100, 100);

    // Unmap the device from memory
    munmap(fbp, screensize);
    close(fbfd);

    return 0;
}
void move_box(box *b, letter *l, int cur_delta_y, int final_delta_y)
{
    if (cur_delta_y == 0)
        return;

    /*if (cur_delta_y % 15 == 0){
      draw_update_letter(l, b->offsety);
      }*/

    /* Delta location jika offeset x/y di increment 1 */
    int offsetx_location_inc = vinfo.bits_per_pixel / 8;
    int offsety_location_inc = finfo.line_length;

    /* Initial location berdasarkan offset x/y box */
    int initial_offsetx_loc = offsetx_location_inc * vinfo.xoffset;
    int initial_offsety_loc = offsety_location_inc * vinfo.yoffset;


    /* Lokasi awal / offset awal */
    long int initial_location = initial_offsetx_loc + ( b->offsetx * offsetx_location_inc )  + initial_offsety_loc + (b->offsety * offsety_location_inc);

    /* finished_location : lokasi setelah iterasi ini berakhir */

    if (final_delta_y > 0) {
        long int finished_location = initial_location + (1 * offsety_location_inc);
        for (int i = b->y - 1; i >= 0; i--) {
            memcpy(fbp + finished_location + (i * offsety_location_inc), fbp + initial_location + (i * offsety_location_inc), b->x*4);
        }
        memset(fbp + initial_location, 0, 4*b->x);
    } else {
        long int finished_location = initial_location - (2 *offsety_location_inc);
        for (int i = 0; i < b->y; i++) {
            memcpy(fbp + finished_location + (i * offsety_location_inc), fbp + initial_location + (i * offsety_location_inc), b->x*4);
        }
        memset(fbp + initial_location + (b->y * offsety_location_inc), 0, 4*b->x);

    }


    if (final_delta_y > 0) {
        b->offsety ++;
        usleep(30000);
        move_box(b, l, cur_delta_y - 1, final_delta_y);
    } else {
        b->offsety --;
        usleep(30000);
        move_box(b, l, cur_delta_y + 1, final_delta_y);
    }

}


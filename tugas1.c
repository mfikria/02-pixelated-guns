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
#define ABS(x) x<0 ? x*(-1) : x


/* Struct definitin */
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



/* Create a box */
void draw_box(box *b);                            /* Draw box to Frame Buffer (FB) */
void draw_letter(letter *b);                      /* Draw letter to FB */
void add_box(letter *, int offsetx, int offsety, int x, int y); /* Add box to a letter */

/* Move box that wrap a letter */
void move_box(box *b, letter *l, int cur_delta_y, int final_delta_y);      /* Rekursif, move box */

void clear_screen();

void print_ahmad(int , int);
void print_tiso(int, int);
void print_try(int, int);
void print_fikri(int base_x, int pos);
void print_fauzan(int base_x, int pos);



/* Global Variable and Macros */
struct fb_var_screeninfo vinfo;         /* Var screen info */
struct fb_fix_screeninfo finfo;         /* Fixed screen info */
int fbfd = 0;                           /* File descriptor */
char *fbp = 0;                          /* pointer to first pixel address in fbfd*/


int main()
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

    int base_x = 40;


    print_tiso(base_x, 0);
    print_try(base_x, 0);
    print_ahmad(base_x, 0);
    print_fikri(base_x, 0);
    print_fauzan(base_x, 0);

    clear_screen();

    // Unmap the device from memory
    munmap(fbp, screensize);
    close(fbfd);

    return 0;
}

void print_fikri(int base_x, int pos) {
    clear_screen();
    letter l;
    l.count = 0;                    /* Initialize box count (MANDATORY) */

    // start from the bottom of the screen
    int i = vinfo.yres - pos -250;


    /* Letter F */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i, 120, 40);
    add_box(&l, base_x + 40, i + 80, 120, 40);
    base_x += 180;

    /* I */
    add_box(&l, base_x, i, 40, 240);
    base_x += 60;

    /* Letter K */
    add_box(&l, base_x, i, 40, 240);
    for(int j=0;j<50;j++)
        add_box(&l, base_x + 40 + j, j+90+i, 1, 85);

    for(int j=0;j<50;j++)
        add_box(&l, base_x + 40 + j, i- j +65, 1, 85);

    for(int j=0;j<=65;j++)
        add_box(&l, base_x + 90 + j, 0 + i + 15, 1, 85 - j);

    for(int j=0;j<=65;j++)
        add_box(&l, base_x + 90 + j, 105 + j + i + 35, 1, 85 - j);

    base_x += 180;

    /* LETTER R */
    add_box(&l, base_x  , i, 40, 240);
    add_box(&l, base_x  , i, 160, 40);
    add_box(&l, base_x , i + 90, 160, 40);
    add_box(&l, base_x + 120 , i, 40, 130);

    int j = 0;
    while (j < 85) {
        add_box(&l, base_x +j + 35, i+130+j, 45, 1);
        j++;
    }
    add_box(&l, base_x + 120 , i + 210, 42, 30);
    base_x += 180;

    /* I */
    add_box(&l, base_x , i, 40, 240);

    draw_letter(&l);
    box letter = {base_x, i, vinfo.xres -10, 240 };
    draw_letter(&l);
    move_box(&letter, &l, -i, -i);

}

void print_ahmad(int base_x, int pos) {
    clear_screen();
    letter l;
    l.count = 0;                    /* Initialize box count (MANDATORY) */

    // start from the bottom of the screen
    int i = vinfo.yres - pos -250;

    /* Letter A */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i, 80, 40);
    add_box(&l, base_x + 40, i + 120, 80, 40);
    add_box(&l, base_x + 120, i, 40, 240);

    /* Letter H */
    add_box(&l, base_x + 180, i, 40, 240);
    add_box(&l, base_x + 220, i + 120, 80, 40);
    add_box(&l, base_x + 300, i, 40, 240);

    add_box(&l, base_x + 360, i, 40, 240);
    add_box(&l, base_x + 400, i, 40, 40);
    add_box(&l, base_x + 440, i, 40, 240);
    add_box(&l, base_x + 480, i, 40, 40);
    add_box(&l, base_x + 520, i, 40, 240);

    add_box(&l, base_x + 580, i, 40, 240);
    add_box(&l, base_x + 620, i, 80, 40);
    add_box(&l, base_x + 620, i + 120, 80, 40);
    add_box(&l, base_x + 700, i, 40, 240);

    add_box(&l, base_x + 760, i, 40, 240);
    add_box(&l, base_x + 800, i, 60, 40);
    add_box(&l, base_x + 800, i + 200, 60, 40);
    add_box(&l, base_x + 860, i + 40, 40, 160);

    /* Finally, draw the letter */
    draw_letter(&l);
    box letter = {base_x, i, vinfo.xres -10, 240 };
    draw_letter(&l);
    move_box(&letter,&l, -i, -i);

}


void print_fauzan(int base_x, int pos) {
    clear_screen();
    letter l;
    l.count = 0;                    /* Initialize box count (MANDATORY) */

    // start from the bottom of the screen
    int i = vinfo.yres - pos -250;

    /* Letter F */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i, 120, 40);
    add_box(&l, base_x + 40, i + 80, 120, 40);

    base_x += 180;

    /* Letter A */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i, 80, 40);
    add_box(&l, base_x + 40, i + 120, 80, 40);
    add_box(&l, base_x + 120, i, 40, 240);
    base_x += 180;

    /* Letter U */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i + 200, 80, 40);
    add_box(&l, base_x + 120, i, 40, 240);

    base_x += 180;

    /* Letter Z */
    add_box(&l, base_x, i, 160, 40);
    add_box(&l, base_x + 120, i+40, 40, 60);
    add_box(&l, base_x, i+100, 160, 40);
    add_box(&l, base_x, i+140, 40, 60);
    add_box(&l, base_x, i+200, 160, 40);

    base_x += 180;

    /* Letter A */
    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 40, i, 80, 40);
    add_box(&l, base_x + 40, i + 120, 80, 40);
    add_box(&l, base_x + 120, i, 40, 240);
    base_x += 180;


    /* Letter N */
    for (int j=0; j<155; j++)
        add_box(&l, base_x+j, i+j, 50,1);

    add_box(&l, base_x, i, 40, 240);
    add_box(&l, base_x + 160, i, 40, 240);

    /* Finally, draw the letter */
    draw_letter(&l);
    box letter = {base_x, i, vinfo.xres -10, 240 };
    draw_letter(&l);
    move_box(&letter,&l, -i, -i);

}

void print_try(int base_x, int pos) {
    clear_screen();
    letter l;
    l.count = 0;

    int i = vinfo.yres - pos -250;

    /* LETTER T */
    add_box(&l, base_x , i, 160, 40);
    add_box(&l, base_x + 65 , i + 40, 40, 200);

    /* LETTER R */
    add_box(&l, base_x + 200 , i, 35, 240);
    add_box(&l, base_x + 200 , i, 160, 35);
    add_box(&l, base_x + 200, i + 90, 160, 35);
    add_box(&l, base_x + 325, i, 35, 125);

    int j = 0;
    while (j < 85) {
        add_box(&l, base_x + 235+j, i+125+j, 45, 1);
        j++;
    }

    add_box(&l, base_x + 321, i + 208, 42, 35);

    for(int k=0;k<80;k++)
        add_box(&l, base_x + 445 + k - 40 , k + i, 1, 65);

    for(int k=0;k<80;k++)
        add_box(&l, base_x + 600 - k - 40, k + i, 1, 65);

    add_box(&l, base_x + 500 - 40, i + 100 , 35, 145);

    draw_letter(&l);
    box letter = {base_x, i, vinfo.xres -10, 240 };
    draw_letter(&l);
    move_box(&letter,&l, -i, -i);

}

void print_tiso(int base_x, int pos) {
    clear_screen();
    letter l;
    l.count = 0;

    int i = vinfo.yres - 250 - pos;

    /* T */
    add_box(&l, base_x, i, 160, 40);
    add_box(&l, base_x + 65 , i + 40, 40, 200);

    /* I */
    add_box(&l, base_x + 180, i, 40, 240);

    /* S */
    add_box(&l, base_x + 240, i, 160, 40);
    add_box(&l, base_x + 240, i + 100, 160, 40);
    add_box(&l, base_x + 240, i + 200, 160, 40);
    add_box(&l, base_x + 240, i + 40, 40, 60);
    add_box(&l, base_x + 360, i + 140, 40, 60);


    /* O */
    add_box(&l, base_x + 420, i, 160, 40);
    add_box(&l, base_x + 420, i + 200, 160, 40);
    add_box(&l, base_x + 420, i + 40, 40, 160);

    add_box(&l, base_x + 540, i + 40, 40, 160);

    box letter = {base_x, i, vinfo.xres -10, 240 };
    draw_letter(&l);
    move_box(&letter,&l, -i, -i);
}

void clear_screen() {
    long int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    memset(fbp, 0, screensize);
}

void draw_box(box *b)
{
    int ix;             /* Sumbu X */
    int iy;             /* Sumbu Y */
    long int location;  /* Point's current location */

    int r = (b->offsetx) % 255;
    r = r < 100 ? 100 : r;
    int g = (b->offsety) % 255;
    g = g < 100 ? 100 : g;
    int bu = ((b->offsetx) + (b->offsety)) % 255;
    bu = bu < 100 ? 100 : bu;

    for (ix = b->offsetx; ix < b->offsetx + b->x; ix++) {
        for (iy = b->offsety; iy < b->offsety + b->y; iy++) {
            location = (ix+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (iy+vinfo.yoffset)*finfo.line_length;
            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = bu;        // Some blue
                *(fbp + location + 1) = g ;     // A little green
                *(fbp + location + 2) = r ;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } else  { //assume 16bpp
                int c = 10;
                int g = (((b->x)-100)/6 )%255;     // A little green
                int r = (31-((b->y)-100)/16) %255;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | c;
                *((unsigned short int*)(fbp + location)) = t;
            }
        }
    }
}

void draw_letter(letter *l)
{
    int i;
    int c = l->count;
    box *box_ptr;
    for (i = 0; i < c; i++) {
        box_ptr=&(l->boxes[i]);
        draw_box(box_ptr);
    }
}

void draw_update_letter(letter *l, int offsety)
{
    int i;
    int c = l->count;
    box *box_ptr;
    for (i = 0; i < c; i++) {
        box_ptr=&(l->boxes[i]);
        box_ptr->offsety = offsety;
        draw_box(box_ptr);
    }
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

    /* Offeset Tujuan */
    int offset_dest;

    /* Setiap Iterasi, current delta di increment */
    if (final_delta_y > 0)                            /* jika delta > 0, offset box ditambah 1 */
        offset_dest = b->offsety + 1;
    else
        offset_dest = b->offsety - 1;

    /* Lokasi awal / offset awal */
    long int initial_location = initial_offsetx_loc + ( b->offsetx * offsetx_location_inc )  + initial_offsety_loc + (b->offsety * offsety_location_inc);

    /* Update offset box */
    b->offsety = offset_dest;

    /* finished_location : lokasi setelah iterasi ini berakhir */
    long int finished_location = initial_offsetx_loc + ( b->offsetx * offsetx_location_inc )  + initial_offsety_loc + (offset_dest * offsety_location_inc);

    memcpy(fbp + finished_location, fbp + initial_location, b->y * offsety_location_inc);
    /* Set warna initial menjadi warna background */
    if (final_delta_y > 0) {
        memset(fbp + initial_location, 0, offsety_location_inc);
        usleep(10000);
        move_box(b, l, cur_delta_y - 1, final_delta_y);
    } else {
        memset(fbp + (b->y + b->offsety) * offsety_location_inc, 0, offsety_location_inc);
        usleep(10000);
        move_box(b, l, cur_delta_y + 1, final_delta_y);
    }
}

void add_box(letter *l, int offsetx, int offsety, int x, int y)
{
    int cur_count = l->count;                   /* Current count */
    box new_box = {offsetx, offsety, x, y};     /* Create new box */
    l->boxes[cur_count] = new_box;              /* Add new box to letter */
    l->count = cur_count + 1;                   /* Increment current box count */
}

void draw_diagonal(letter *l, int i, int base_x) {
    int j;
    for(j=0;j<85;j++) {
        add_box(l, base_x + 235+j, i + 125 + j, 45, 1);
    }

}


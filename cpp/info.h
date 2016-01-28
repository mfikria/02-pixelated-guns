#include <linux/fb.h>

class Info {
    public:
        Info(); 
        static int xres;            /* Resolusi horizontal layar */
        static int yres;            /* Resolusi vertical layar */
        static int bytePerX;        /* Jumlah byte untuk menggeser 1 pixel horizontal */
        static int bytePerY;        /* Jumlah byte untuk menggerer 1 pixel vertical */
        
        static long int screensize;

        static struct fb_var_screeninfo vinfo;         /* Var screen info */
        static struct fb_fix_screeninfo finfo;         /* Fixed screen info */
        static int fbfd;                           /* File descriptor */
        static char *fbp;                          /* pointer to first pixel address in fbfd*/

};

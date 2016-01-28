#ifndef _THING_
#define _THING_

#define MAX_BOX 4096
#define SPEED 15000
/* Struktur Box */
typedef struct Box {
    int offX;       /* Offset X */
    int offY;       /* Offest Y */
    int x;          /* Panjang X */
    int y;          /* Lebar Y */
    int rgb[3];     /* Array of int, rgb[0] = red, rgb[1] = green, rgb[2] = blue */
} Box;


/* Class for everthing */
class Thing {
    private:
        Box boxes[MAX_BOX];      /* Array of struct Box */
        int nBox;                /* Current number of box */

        int offX;               /* Offset X dari Thing */
        int offY;               /* Offest Y dari Thing */
        int x;                  /* Panjang Thing */
        int y;                  /* Lebar Thing */
        
        int maxX;
        int minX;
        int maxY;
        int minY;

    public:
        Thing();

        Box *getEl(int i);      /* Akses pada elemen subs (array of box) */


        /* Getters */
        int getOffX();          
        int getOffY();
        int getX();
        int getY();

        /* Setters */
        void addBox(int offX, int offY, int x, int y, int rgb[3]);
        void addBox(Box *box);

        /* Thing's method, yang berhubungan dengan penggambaran ke layar */
        void draw(); /* Gambar ke layar*/
        void moveX(int dx);                                         /* Move thing sepanjang dx */
        void moveY(int dy);                                         /* Move thing sepanjang dy */
};
#endif
